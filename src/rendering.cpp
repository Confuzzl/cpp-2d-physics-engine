module;

#include "util/gl.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <algorithm>
#include <fstream>

module rendering;

import <iostream>;
import <format>;
import <vector>;

import glm;
import debug;
import vertices;
import buffer_objects;
import mesh;
import app;

static const glm::lowp_u16vec2 QUAD_UVS[2][3]{{{0, 0}, {1, 0}, {1, 1}},
                                              {{0, 0}, {1, 1}, {0, 1}}};

const glm::mat4 Renderer::UI_MATRIX{
    glm::ortho(0.0f, static_cast<float>(App::WIDTH), 0.0f,
               static_cast<float>(App::HEIGHT))};

Renderer::Renderer() {}
void Renderer::init() {
  initFontTexture();
  shapeShader.create();
  fontShader.create();
}

void Renderer::initFontTexture() {
  glCreateTextures(GL_TEXTURE_2D, 1, &fontTexture);
  glTextureParameteri(fontTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTextureParameteri(fontTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  stbi_set_flip_vertically_on_load(true);
  int width, height, channels;
  unsigned char *data = stbi_load("assets/consolas1024.png", &width, &height,
                                  &channels, STBI_rgb_alpha);
  if (!data) {
    println("IMAGE FAILED TO LOAD");
  }
  glTextureStorage2D(fontTexture, 1, GL_RGBA8, width, height);
  glTextureSubImage2D(fontTexture, 0, 0, 0, width, height, GL_RGBA,
                      GL_UNSIGNED_BYTE, data);
  stbi_image_free(data);
}

void Renderer::renderFrame(const double t) const {
  glClearColor(0, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  renderText(t);
  renderScene(t);

  glfwSwapBuffers(MAIN_APP.window);
}
void Renderer::renderScene(const double t) const {
  for (const std::unique_ptr<Object> &obj : MAIN_SCENE.objects) {
    render(*obj, {.showAABB = true});
  }
}
void Renderer::renderText(const double t) const {
  text(std::format("t: {:.2f}s", glfwGetTime()), 0, 0);
  text(std::format("z: {:.2f}", MAIN_CAMERA.zoom), 0, 100);
}

static unsigned short charWidthConvert(const unsigned char w) {
  return static_cast<unsigned short>(
      static_cast<float>(w) * Renderer::CHAR_WIDTH / Renderer::FONT_WIDTH *
      Renderer::TEXEL_RANGE);
}
static unsigned short charHeightConvert(const unsigned char h) {
  return static_cast<unsigned short>(
      static_cast<float>(h) * Renderer::CHAR_HEIGHT / Renderer::FONT_HEIGHT *
      Renderer::TEXEL_RANGE);
}

void Renderer::text(const std::string &str, const unsigned short x,
                    const unsigned short y, const double scale) const {
  const GLuint vertexCount = 6 * static_cast<GLuint>(str.size());

  std::vector<FontVertex> vertices{};
  vertices.reserve(vertexCount);

  unsigned short xOffset = x;
  for (const char c : str) {
    const char id = c - 32;
    const unsigned char row = id / COLUMNS, column = id % COLUMNS;

    const double width = CHAR_WIDTH * scale, height = CHAR_HEIGHT * scale;

    glm::vec4 uvInfo{column * CHAR_WIDTH, row * CHAR_HEIGHT, CHAR_WIDTH,
                     CHAR_HEIGHT};
    uvInfo[1] = FONT_HEIGHT - uvInfo[0] - uvInfo[3];
    uvInfo /= glm::vec4{FONT_WIDTH, FONT_HEIGHT, FONT_WIDTH, FONT_HEIGHT};
    uvInfo *= glm::vec4{SHRT_MAX};

    const glm::lowp_u16vec2 uvCoordinates{uvInfo[0], uvInfo[1]};
    const glm::lowp_u16vec2 uvDimensions{uvInfo[2], uvInfo[3]};

    for (auto tri = 0; tri < 2; tri++) {
      for (auto v = 0; v < 3; v++) {
        const glm::vec2 pos{xOffset + width * QUAD_UVS[tri][v][0],
                            y + height * QUAD_UVS[tri][v][1]};
        const glm::lowp_u16vec2 uv = QUAD_UVS[tri][v];
        vertices.emplace_back(
            pos[0], pos[1],
            charWidthConvert(column) + uv[0] * charWidthConvert(1),
            charHeightConvert(ROWS - row - 1) + uv[1] * charHeightConvert(1));
      }
    }

    xOffset += static_cast<unsigned short>(CHAR_WIDTH * scale);
  }

  VBO<FontVertex> vbo{vertexCount};

  GLintptr offset = 0;
  for (const auto &vertex : vertices) {
    glNamedBufferSubData(vbo.ID, offset, sizeof(vertex.pos), vertex.pos);
    offset += sizeof(vertex.pos);
    glNamedBufferSubData(vbo.ID, offset, sizeof(vertex.tex), vertex.tex);
    offset += sizeof(vertex.tex);
  }
  glVertexArrayVertexBuffer(fontShader.vao, 0, vbo.ID, 0, sizeof(FontVertex));

  glUseProgram(fontShader.ID);
  fontShader.setProjection(UI_MATRIX);

  glBindTextureUnit(0, fontTexture);
  glBindVertexArray(fontShader.vao);

  glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

import object;

void Renderer::render(const Object &object, render_opts &&opts) const {
  const Mesh &mesh = *object.mesh;

  glUseProgram(shapeShader.ID);
  glBindVertexArray(shapeShader.vao);
  glVertexArrayElementBuffer(shapeShader.vao, mesh.ebo.ID);
  glVertexArrayVertexBuffer(shapeShader.vao, 0, mesh.vbo.ID, 0,
                            sizeof(ShapeVertex));

  GLintptr offset = 0;
  for (const ShapeVertex &vertex : mesh.localVertexData) {
    glNamedBufferSubData(mesh.vbo.ID, offset, sizeof(vertex), vertex.pos);
    offset += sizeof(vertex);
  }
  shapeShader.setParentPos(object.properties.pos);
  shapeShader.setRotation(object.properties.rotation);
  shapeShader.setView(MAIN_SCENE.camera.getView());

  shapeShader.setFragColor(object.getColor());

  glDrawElements(opts.primitive, mesh.ebo.count, GL_UNSIGNED_BYTE, 0);

  if (opts.showAABB)
    render(*object.collider->aabb, object.getColor());
}

import aabb;

void Renderer::render(const AABB &aabb, const glm::uvec3 &color) const {
  glUseProgram(shapeShader.ID);
  glBindVertexArray(shapeShader.vao);
  glVertexArrayVertexBuffer(shapeShader.vao, 0, aabb.vbo.ID, 0,
                            sizeof(glm::vec2));

  GLintptr offset = 0;
  for (const glm::vec2 &vertex : aabb.corners()) {
    glNamedBufferSubData(aabb.vbo.ID, offset, sizeof(vertex),
                         glm::value_ptr(vertex));
    offset += sizeof(vertex);
  }
  shapeShader.setParentPos(aabb.parent.globalPos());
  shapeShader.setView(MAIN_SCENE.camera.getView());

  shapeShader.setFragColor(color);

  glDrawArrays(GL_LINE_LOOP, 0, 4);
}
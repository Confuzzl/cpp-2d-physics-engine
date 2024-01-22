module;

#include "gl.h"
#include <format>
#include <fstream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

#include <glm/gtc/type_ptr.hpp>

module rendering;

import <glm/glm.hpp>;
import debug;

static std::string sourceToString(const std::string &name) {
  std::ifstream in{name};
  return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

static const glm::lowp_u16vec2 QUAD_UVS[2][3]{{{0, 0}, {1, 0}, {1, 1}},
                                              {{0, 0}, {1, 1}, {0, 1}}};

const glm::mat4 Renderer::UI_MATRIX{glm::ortho(
    0.0f, static_cast<float>(WIDTH), 0.0f, static_cast<float>(HEIGHT))};

Renderer::Renderer() {
  createShader(shapeShader, "shape");
  glCreateVertexArrays(1, &shapeVAO);
  glEnableVertexArrayAttrib(shapeVAO, 0);
  glVertexArrayAttribFormat(shapeVAO, 0, 2, GL_FLOAT, false, 0);
  glVertexArrayAttribBinding(shapeVAO, 0, 0);

  createShader(fontShader, "font");
  glCreateVertexArrays(1, &fontVAO);
  glEnableVertexArrayAttrib(fontVAO, 0);
  glVertexArrayAttribFormat(fontVAO, 0, 2, GL_FLOAT, false, 0);
  glVertexArrayAttribBinding(fontVAO, 0, 0);
  glEnableVertexArrayAttrib(fontVAO, 1);
  glVertexArrayAttribFormat(fontVAO, 1, 2, GL_UNSIGNED_SHORT, false,
                            2 * sizeof(GLfloat));
  glVertexArrayAttribBinding(fontVAO, 1, 0);

  initFontTexture();
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

void Renderer::compileShader(const GLenum type, GLuint &ID,
                             const std::string &source) {
  GLint success = 0;
  ID = glCreateShader(type);

  std::string temp = sourceToString(source);
  const char *chars = temp.c_str();
  glShaderSource(ID, 1, &chars, NULL);

  glCompileShader(ID);
  glGetShaderiv(ID, GL_COMPILE_STATUS, &success);

  if (!success) {
    println("COMPILATION ERROR {}", source);
  }
  println("{} {}", source, ID);
}

void Renderer::createShader(GLuint &ID, const std::string &name) {
  ID = glCreateProgram();

  GLuint vertID = 0, fragID = 0;
  compileShader(GL_VERTEX_SHADER, vertID, std::format("assets/{}.vert", name));
  compileShader(GL_FRAGMENT_SHADER, fragID,
                std::format("assets/{}.frag", name));

  glAttachShader(ID, vertID);
  glAttachShader(ID, fragID);
  glLinkProgram(ID);
  glDeleteShader(vertID);
  glDeleteShader(fragID);
}

struct FontVertex {
  GLfloat pos[2];
  GLushort tex[2];

  FontVertex(GLfloat x, GLfloat y, GLushort u, GLushort v)
      : pos{x, y}, tex{u, v} {}
};

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
                    const unsigned short y) const {
  const GLuint vertexCount = 6 * static_cast<GLuint>(str.size());

  std::vector<FontVertex> vertices{};
  vertices.reserve(vertexCount);

  unsigned short xOffset = x;
  for (const char c : str) {
    const char id = c - 32;
    const unsigned char row = id / COLUMNS, column = id % COLUMNS;
    // println("{} {}", row, column);
    // println("{} {} {} {}", charWidthConvert(column),
    //         charHeightConvert(ROWS - row), charWidthConvert(column + 1),
    //         charHeightConvert(ROWS - (row + 1)));

    glm::vec4 uvInfo{column * CHAR_WIDTH, row * CHAR_HEIGHT, CHAR_WIDTH,
                     CHAR_HEIGHT};
    uvInfo[1] = FONT_HEIGHT - uvInfo[0] - uvInfo[3];
    uvInfo /= glm::vec4{FONT_WIDTH, FONT_HEIGHT, FONT_WIDTH, FONT_HEIGHT};
    uvInfo *= glm::vec4{SHRT_MAX};

    const glm::lowp_u16vec2 uvCoordinates{uvInfo[0], uvInfo[1]};
    const glm::lowp_u16vec2 uvDimensions{uvInfo[2], uvInfo[3]};

    for (auto tri = 0; tri < 2; tri++) {
      for (auto v = 0; v < 3; v++) {
        const glm::vec2 pos{xOffset + CHAR_WIDTH * QUAD_UVS[tri][v][0],
                            y + CHAR_HEIGHT * QUAD_UVS[tri][v][1]};
        const glm::lowp_u16vec2 uv{/*uvCoordinates +
                                   uvDimensions * */
                                   QUAD_UVS[tri][v]};
        vertices.emplace_back(
            pos[0], pos[1],
            charWidthConvert(column) + uv[0] * charWidthConvert(1),
            charHeightConvert(ROWS - row - 1) + uv[1] * charHeightConvert(1));
      }
    }

    xOffset += CHAR_WIDTH;
  }

  GLuint vbo;
  glCreateBuffers(1, &vbo);
  glNamedBufferStorage(vbo, vertexCount * sizeof(FontVertex), NULL,
                       GL_DYNAMIC_STORAGE_BIT);
  GLintptr offset = 0;
  for (const auto &vertex : vertices) {
    glNamedBufferSubData(vbo, offset, sizeof(vertex.pos), vertex.pos);
    offset += sizeof(vertex.pos);
    glNamedBufferSubData(vbo, offset, sizeof(vertex.tex), vertex.tex);
    offset += sizeof(vertex.tex);
  }
  glVertexArrayVertexBuffer(fontVAO, 0, vbo, 0, sizeof(FontVertex));

  glUseProgram(fontShader);
  glUniformMatrix4fv(glGetUniformLocation(fontShader, "projection"), 1,
                     GL_FALSE, glm::value_ptr(UI_MATRIX));
  glBindTextureUnit(0, fontTexture);
  glBindVertexArray(fontVAO);

  glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void Renderer::render(const Mesh &mesh) const {
  // GLuint vbo =
  for (const auto &vertex : mesh.data) {
  }

  glUseProgram(shapeShader);
  glBindVertexArray(shapeVAO);

  // glDrawElements
}
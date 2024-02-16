module;

#include "util/gl.h"

module rendering;

import buffer_objects;
import vertices;
import app;
import <numbers>;

void Renderer::drawGrid() const {
  static constexpr int HALF_SIZE = 3;
  static constexpr float WIDTH = 1;
  static constexpr unsigned int AXIS_COUNT = 2 * (HALF_SIZE * 2 + 1);
  static constexpr unsigned int VERTEX_COUNT = AXIS_COUNT * AXIS_COUNT;
  static vbo<ShapeVertex> VBO{VERTEX_COUNT};
  static const glm::vec2 AXES[]{{1.0f, 0.0f}, {0.0f, 1.0f}};

  glUseProgram(shapeShader.ID);
  glBindVertexArray(shapeShader.vao);
  glVertexArrayVertexBuffer(shapeShader.vao, 0, VBO.ID, 0, sizeof(glm::vec2));

  GLintptr offset = 0;
  for (int a = 0; a < 2; a++) {
    const glm::vec2 &axis = AXES[a];
    const glm::vec2 &other = AXES[1 - a];
    for (int i = -HALF_SIZE; i <= +HALF_SIZE; i++) {
      const glm::vec2 perpOffset = other * static_cast<float>(i);

      glNamedBufferSubData(
          VBO.ID, offset, sizeof(glm::vec2),
          glm::value_ptr(glm::vec2{axis * static_cast<float>(-HALF_SIZE)} +
                         perpOffset));
      offset += sizeof(glm::vec2);
      glNamedBufferSubData(
          VBO.ID, offset, sizeof(glm::vec2),
          glm::value_ptr(glm::vec2{axis * static_cast<float>(+HALF_SIZE)} +
                         perpOffset));
      offset += sizeof(glm::vec2);
    }
  }
  shapeShader.setParentPos({0, 0});
  shapeShader.setView(MAIN_SCENE.camera.getView());
  shapeShader.setFragColor({127, 127, 127});
  glDrawArrays(GL_LINES, 0, VERTEX_COUNT);
}

void Renderer::drawPoint(const glm::vec2 &point,
                         const glm::uvec3 &color) const {
  static vbo<ShapeVertex> VBO{1};

  glUseProgram(shapeShader.ID);
  glBindVertexArray(shapeShader.vao);
  glVertexArrayVertexBuffer(shapeShader.vao, 0, VBO.ID, 0, sizeof(glm::vec2));

  glNamedBufferSubData(VBO.ID, 0, sizeof(glm::vec2), glm::value_ptr(point));

  shapeShader.setParentPos({0, 0});
  shapeShader.setView(MAIN_SCENE.camera.getView());
  shapeShader.setFragColor(color);
  glDrawArrays(GL_POINTS, 0, 1);
}
void Renderer::drawLine(const glm::vec2 &a, const glm::vec2 &b,
                        const glm::uvec3 &color) const {
  static vbo<ShapeVertex> VBO{2};

  glUseProgram(shapeShader.ID);
  glBindVertexArray(shapeShader.vao);
  glVertexArrayVertexBuffer(shapeShader.vao, 0, VBO.ID, 0, sizeof(glm::vec2));

  glNamedBufferSubData(VBO.ID, 0, sizeof(glm::vec2), glm::value_ptr(a));
  glNamedBufferSubData(VBO.ID, sizeof(glm::vec2), sizeof(glm::vec2),
                       glm::value_ptr(b));

  shapeShader.setParentPos({0, 0});
  shapeShader.setView(MAIN_SCENE.camera.getView());
  shapeShader.setFragColor(color);
  glDrawArrays(GL_LINES, 0, 2);
}
void Renderer::drawArrow(const glm::vec2 &origin, const glm::vec2 &vector,
                         const glm::uvec3 &color) const {
  drawArrowFromTo(origin, origin + vector, color);
}
void Renderer::drawArrowFromTo(const glm::vec2 &from, const glm::vec2 &to,
                               const glm::uvec3 &color) const {
  //   1
  //  /|\
  // 2 | 3
  //   |
  //   0
  static vbo<ShapeVertex> VBO{4};
  static simple_ebo EBO{{0, 1, 1, 2, 1, 3}};

  static constexpr float HEAD_SIZE = 0.5f;
  static constexpr float ANGLE_OFFSET = std::numbers::pi_v<float> / 4;
  static const float COS = std::cosf(ANGLE_OFFSET),
                     SIN = std::sinf(ANGLE_OFFSET);

  const glm::vec2 baseHead = glm::normalize(from - to) * HEAD_SIZE;
  const float xcos = baseHead.x * COS;
  const float ysin = baseHead.y * SIN;
  // a > 0
  // (x * cos a - y * sin a, x * cos a + y * sin a)
  // a < 0
  // (x * cos a + y * sin a, x * cos a - y * sin a)
  const glm::vec2 vertices[4] = {
      from, to, {xcos + ysin, xcos - ysin}, {xcos - ysin, xcos + ysin}};

  GLintptr offset = 0;
  for (const glm::vec2 &vertex : vertices) {
    glNamedBufferSubData(VBO.ID, offset, sizeof(vertex),
                         glm::value_ptr(vertex));
    offset += sizeof(vertex);
  }

  glUseProgram(shapeShader.ID);
  glBindVertexArray(shapeShader.vao);
  glVertexArrayElementBuffer(shapeShader.vao, EBO.ID);
  glVertexArrayVertexBuffer(shapeShader.vao, 0, VBO.ID, 0, sizeof(glm::vec2));

  shapeShader.setParentPos({0, 0});
  shapeShader.setView(MAIN_SCENE.camera.getView());
  shapeShader.setFragColor(color);

  glDrawElements(GL_LINES, EBO.count, GL_UNSIGNED_BYTE, 0);
}

void Renderer::drawCircle(const glm::vec2 &center, const float radius,
                          const glm::uvec3 &color) const {
  glUseProgram(circleShader.ID);
  glBindVertexArray(circleShader.vao);

  circleShader.setCenter(center);
  circleShader.setRadius(radius);
  circleShader.setFragColor(color);

  glDrawArrays(GL_TRIANGLES, 0, 1);
}
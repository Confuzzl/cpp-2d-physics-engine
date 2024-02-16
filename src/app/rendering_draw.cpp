module;

#include "util/gl.h"

module rendering;

import buffer_objects;
import vertices;
import app;

void Renderer::drawGrid() const {
  static constexpr int HALF_SIZE = 3;
  static constexpr float WIDTH = 1;
  static constexpr unsigned int AXIS_COUNT = 2 * (HALF_SIZE * 2 + 1);
  static constexpr unsigned int VERTEX_COUNT = AXIS_COUNT * AXIS_COUNT;
  static VBO<ShapeVertex> VBO{VERTEX_COUNT};
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

void Renderer::drawPoint(const glm::vec2 &point) const {
  static VBO<ShapeVertex> VBO{1};

  glUseProgram(shapeShader.ID);
  glBindVertexArray(shapeShader.vao);
  glVertexArrayVertexBuffer(shapeShader.vao, 0, VBO.ID, 0, sizeof(glm::vec2));

  glNamedBufferSubData(VBO.ID, 0, sizeof(glm::vec2), glm::value_ptr(point));

  shapeShader.setParentPos({0, 0});
  shapeShader.setView(MAIN_SCENE.camera.getView());
  glDrawArrays(GL_POINTS, 0, 1);
}
void Renderer::drawLine(const glm::vec2 &a, const glm::vec2 &b) const {}
void Renderer::drawArrow(const glm::vec2 &origin,
                         const glm::vec2 &direction) const {}
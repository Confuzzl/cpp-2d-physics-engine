module;

#include "util/gl.h"

export module rendering;

import <string>;
import glm;

import shader;

export struct Object;
export struct AABB;

struct render_opts {
  bool showAABB = false;
  GLenum primitive = GL_TRIANGLES;
};

export struct Renderer {
  static constexpr unsigned short TEXEL_RANGE = 1 << 15;
  static constexpr unsigned short FONT_WIDTH = 1024, FONT_HEIGHT = 1024;
  static constexpr unsigned char CHAR_WIDTH = 64, CHAR_HEIGHT = 128;
  static constexpr unsigned char COLUMNS = 16, ROWS = 8;
  static const glm::mat4 UI_MATRIX;

  ShapeShader shapeShader{};
  FontShader fontShader{};

  GLuint fontTexture = 0;

  Renderer();
  /*call after glfwinit*/
  void init();

private:
  void initFontTexture();
  void compileShader(const GLenum type, GLuint &ID, const std::string &source);
  void createShader(GLuint &ID, const std::string &name);

public:
  void renderFrame(const double t) const;
  void renderScene(const double t) const;
  void renderText(const double t) const;

  void text(const std::string &str, const unsigned short x = 0,
            const unsigned short y = 0, const double scale = 0.25) const;

  void render(const Object &object, render_opts &&opts) const;
  void render(const AABB &aabb, const glm::uvec3 &color) const;

  void drawGrid() const;
  void drawPoint(const glm::vec2 &point) const;
  void drawLine(const glm::vec2 &a, const glm::vec2 &b) const;
  void drawArrow(const glm::vec2 &origin, const glm::vec2 &direction) const;
};
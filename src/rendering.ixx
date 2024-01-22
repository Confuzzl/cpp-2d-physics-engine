module;

#include "gl.h"

export module rendering;

import <string>;
import <glm/glm.hpp>;
import <glm/gtc/matrix_transform.hpp>;

export struct Renderer {
  static constexpr GLsizei WIDTH = 1280, HEIGHT = 720;
  static constexpr unsigned short TEXEL_RANGE = 1 << 15;
  static constexpr unsigned short FONT_WIDTH = 1024, FONT_HEIGHT = 1024;
  static constexpr unsigned char CHAR_WIDTH = 64, CHAR_HEIGHT = 128;
  static constexpr unsigned char COLUMNS = 16, ROWS = 8;
  static const glm::mat4 UI_MATRIX;

  GLuint shapeShader = 0, fontShader = 0;
  GLuint shapeVAO = 0, fontVAO = 0;

  GLuint fontTexture = 0;

  Renderer();

  void initFontTexture();

  void compileShader(const GLenum type, GLuint &ID, const std::string &source);
  void createShader(GLuint &ID, const std::string &name);

  void text(const std::string &str, const unsigned short x = 0,
            const unsigned short y = 0) const;
};
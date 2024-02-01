module;

#include "gl.h"
#include <fstream>

module shader;

import debug;
import glm;

static std::string sourceToString(const std::string &name) {
  std::ifstream in{name};
  return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

Shader::Shader(const std::string &name) {
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

void Shader::compileShader(const GLenum type, GLuint &ID,
                           const std::string &source) {
  GLint success = 0;
  ID = glCreateShader(type);

  std::string temp = sourceToString(source);
  const char *chars = temp.c_str();
  glShaderSource(ID, 1, &chars, NULL);

  glCompileShader(ID);
  glGetShaderiv(ID, GL_COMPILE_STATUS, &success);

  if (!success) {
    GLint length;
    glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &length);
    std::string log = "";
    log.resize(length);
    glGetShaderInfoLog(ID, length, NULL, log.data());
    println("COMPILATION ERROR {}", source);
    println(log);
    return;
  }
  println("{} {}", source, ID);
}

FontShader::FontShader()
    : Shader("font"), projection{ID, "projection"},
      font_color{ID, "font_color"} {
  glCreateVertexArrays(1, &vao);
  glEnableVertexArrayAttrib(vao, 0);
  glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, false, 0);
  glVertexArrayAttribBinding(vao, 0, 0);
  glEnableVertexArrayAttrib(vao, 1);
  glVertexArrayAttribFormat(vao, 1, 2, GL_UNSIGNED_SHORT, false,
                            2 * sizeof(GLfloat));
  glVertexArrayAttribBinding(vao, 1, 0);
}

void FontShader::setProjection(const glm::mat4 &matrix) const {
  setUniform(projection, matrix);
}
void FontShader::setFontColor(const glm::uvec3 &color) const {
  setUniform(font_color, color);
}

ShapeShader::ShapeShader()
    : Shader("shape"), parent_pos{ID, "parent_pos"}, rotation{ID, "rotation"},
      view{ID, "view"}, frag_color{ID, "frag_color"} {
  glCreateVertexArrays(1, &vao);
  glEnableVertexArrayAttrib(vao, 0);
  glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, false, 0);
  glVertexArrayAttribBinding(vao, 0, 0);
}

void ShapeShader::setParentPos(const glm::vec2 &pos) const {
  setUniform(parent_pos, pos);
}
void ShapeShader::setRotation(const float value) const {
  setUniform(rotation, value);
}
void ShapeShader::setView(const glm::mat4 &matrix) const {
  setUniform(view, matrix);
}
void ShapeShader::setFragColor(const glm::uvec3 &color) const {
  setUniform(frag_color, color);
}
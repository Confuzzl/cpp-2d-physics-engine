module;

#include "gl.h"

export module shader;

import <stdexcept>;

import debug;
import glm;

template <typename T> struct uniform {
  GLint location;

  void create(GLuint shaderID, const std::string &name) {
    const GLint loc = glGetUniformLocation(shaderID, name.c_str());
    if (loc == -1)
      throw std::runtime_error{
          std::format("{} was not a valid uniform name", name)};
    location = loc;
  }
};

export struct Shader {
  GLuint ID;
  GLuint vao;

  const std::string name;

protected:
  Shader(const std::string &name);

private:
  void createShaders();
  void compileShader(const GLenum type, GLuint &ID, const std::string &source);
  virtual void createVAO() = 0;
  virtual void createUniforms() = 0;

public:
  void create();

  template <typename T>
  void setUniform(const uniform<T> &uniform, const T &value) const;
  template <>
  void setUniform<float>(const uniform<float> &uniform,
                         const float &value) const {
    glUniform1f(uniform.location, value);
  }
  template <>
  void setUniform<glm::mat4>(const uniform<glm::mat4> &uniform,
                             const glm::mat4 &matrix) const {
    glUniformMatrix4fv(uniform.location, 1, GL_FALSE, glm::value_ptr(matrix));
  }
  template <>
  void setUniform<glm::vec2>(const uniform<glm::vec2> &uniform,
                             const glm::vec2 &vector) const {
    glUniform2fv(uniform.location, 1, glm::value_ptr(vector));
  }
  template <>
  void setUniform<glm::uvec3>(const uniform<glm::uvec3> &uniform,
                              const glm::uvec3 &vector) const {
    glUniform3uiv(uniform.location, 1, glm::value_ptr(vector));
  }
};

export struct FontShader : public Shader {
  uniform<glm::mat4> projection;
  uniform<glm::uvec3> font_color;

  FontShader();

private:
  void createVAO() override;
  void createUniforms() override;

public:
  void setProjection(const glm::mat4 &matrix) const;
  void setFontColor(const glm::uvec3 &color) const;
};

export struct ShapeShader : public Shader {
  uniform<glm::vec2> parent_pos;
  uniform<float> rotation;
  uniform<glm::mat4> view;
  uniform<glm::uvec3> frag_color;

  ShapeShader();

private:
  void createVAO() override;
  void createUniforms() override;

public:
  void setParentPos(const glm::vec2 &pos) const;
  void setRotation(const float value) const;
  void setView(const glm::mat4 &matrix) const;
  void setFragColor(const glm::uvec3 &color) const;
};
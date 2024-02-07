module;

#include "util/gl.h"

export module buffer_objects;

import vertices;
import <vector>;
import <numeric>;
import debug;

struct BufferObject {
  GLuint ID;
  BufferObject() { glCreateBuffers(1, &ID); }
};

export template <typename T> struct VBO : public BufferObject {
  VBO(const unsigned int n) : BufferObject() {
    glNamedBufferStorage(ID, n * sizeof(T), NULL, GL_DYNAMIC_STORAGE_BIT);
  }
};

export struct RadialEBO : public BufferObject {
  const unsigned int count;
  RadialEBO(const unsigned char n) : BufferObject(), count{n * 3u} {
    std::vector<GLubyte> indices{};
    indices.reserve(count);
    for (int i = 0; i < n - 2; i++) {
      indices.emplace_back(0);
      indices.emplace_back(i + 1);
      indices.emplace_back(i + 2);
    }
    indices.emplace_back(0);
    indices.emplace_back(n - 1);
    indices.emplace_back(1);
    glNamedBufferData(ID, count * sizeof(GLubyte), indices.data(),
                      GL_STATIC_DRAW);
  }
};
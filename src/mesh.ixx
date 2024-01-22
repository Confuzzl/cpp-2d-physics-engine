module;

#include "gl.h"

export module mesh;

import <vector>;

struct ShapeVertex {
  GLfloat pos[2];
  ShapeVertex(const GLfloat x, const GLfloat y) : pos{x, y} {}
};

export struct Mesh {
  std::vector<ShapeVertex> data;

  Mesh(std::vector<ShapeVertex> &&data) : data{std::move(data)} {}

  virtual GLuint getVBO() const = 0;
  virtual GLuint getEBO() const = 0;
};

export template <unsigned short N> struct PolyMesh : public Mesh {
  static_assert(N >= 3);

  PolyMesh(std::vector<ShapeVertex> &&data) : Mesh(std::move(data)) {}

  static GLuint createVBO() {
    GLuint vbo;
    glCreateBuffers(1, &vbo);
    glNamedBufferStorage(vbo, N * sizeof(ShapeVertex), NULL,
                         GL_DYNAMIC_STORAGE_BIT);
    return vbo;
  }

  static GLuint createEBO() {}

  GLuint getVBO() const override {
    static GLuint vbo = createVBO();
    return vbo;
  }
  GLuint getEBO() const override {
    static GLuint ebo = 0;
    return ebo;
  }

  static PolyMesh<N> create(const double radius, const double theta = 0) {
    std::vector<ShapeVertex> data{};
    data.reserve(N);
    return {std::move(data)};
  }
};
export struct CircleMesh : public Mesh {
  static constexpr unsigned char SEGMENTS = 16;

  CircleMesh(std::vector<ShapeVertex> &&data) : Mesh(std::move(data)) {}
  GLuint getVBO() const override {
    static GLuint vbo = 0;
    return vbo;
  }
  GLuint getEBO() const override {
    static GLuint ebo = 0;
    return ebo;
  }

  static CircleMesh create(const double radius) {
    std::vector<ShapeVertex> data{};
    data.reserve(1 + SEGMENTS);
  }
};
module;

#include "gl.h"

export module vertices;

export struct ShapeVertex {
  GLfloat pos[2];
  ShapeVertex(const GLfloat x, const GLfloat y) : pos{x, y} {}
};

export struct FontVertex {
  GLfloat pos[2];
  GLushort tex[2];

  FontVertex(GLfloat x, GLfloat y, GLushort u, GLushort v)
      : pos{x, y}, tex{u, v} {}
};
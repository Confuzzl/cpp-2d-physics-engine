module;

#include "util/gl.h"
#include <concepts>

export module mesh;

import <vector>;

import poly;
import vertex_layout;
import bo_heap;
import color;

export struct Mesh {
  std::vector<vertex_layout::pos> data;

  EBOHandle ebo;

  Color color;
  GLenum primitive;

  Mesh(std::vector<vertex_layout::pos> &&data, const GLenum primitive,
       const Color color)
      : data{std::move(data)}, primitive{primitive}, color{color} {}
  Mesh(std::vector<vertex_layout::pos> &&data, EBOHandle &&ebo,
       const GLenum primitive, const Color color)
      : data{std::move(data)}, ebo{std::move(ebo)}, primitive{primitive},
        color{color} {}
};
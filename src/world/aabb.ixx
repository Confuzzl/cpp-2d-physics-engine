export module aabb;

import glm;
import buffer_objects;
import vertices;

import math;

import <array>;

export struct Collider;

export struct AABB {
  const VBO<ShapeVertex> &vbo;

  const Collider &parent;

  glm::vec2 min{}, max{};

  AABB(const Collider &parent, const glm::vec2 &min = {F_INF_POS, F_INF_POS},
       const glm::vec2 &max = {F_INF_NEG, F_INF_NEG});

  void expand(const glm::vec2 &p);

  std::array<glm::vec2, 4> corners() const;

  glm::vec2 globalPos() const;
};
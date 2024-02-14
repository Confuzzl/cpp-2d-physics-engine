module;

#include <ranges>

export module aabb;

import glm;
import buffer_objects;
import vertices;

import math;

import <array>;
import <vector>;
import <memory>;

export struct Collider;

export struct AABB {
  const VBO<ShapeVertex> &vbo;

  const Collider &parent;

private:
  glm::vec2 min{}, max{};

public:
  glm::vec2 localMin() const { return min; }
  glm::vec2 globalMin() const { return min + globalPos(); }
  glm::vec2 localMax() const { return max; }
  glm::vec2 globalMax() const { return max + globalPos(); }

  AABB(const Collider &parent, const glm::vec2 &min = {F_INF_POS, F_INF_POS},
       const glm::vec2 &max = {F_INF_NEG, F_INF_NEG});

  static std::unique_ptr<AABB> from(const Collider &parent, const float radius);
  static std::unique_ptr<AABB> from(const Collider &parent,
                                    const std::vector<glm::vec2> vertices);

  bool intersects(const AABB &other) const;
  void expand(const glm::vec2 &p);

  std::array<glm::vec2, 4> localVertices() const;
  auto globalVertices() const {
    return localVertices() |
           std::views::transform([this](const glm::vec2 &vertex) {
             return vertex + this->globalPos();
           });
  }

  glm::vec2 globalPos() const;
};
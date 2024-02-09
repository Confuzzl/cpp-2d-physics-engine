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
  const glm::vec2 &getMin() const { return min; }
  const glm::vec2 &getMax() const { return max; }

  AABB(const Collider &parent, const glm::vec2 &min = {F_INF_POS, F_INF_POS},
       const glm::vec2 &max = {F_INF_NEG, F_INF_NEG});

  static std::unique_ptr<AABB> from(const Collider &parent, const float radius);
  static std::unique_ptr<AABB> from(const Collider &parent,
                                    const std::vector<glm::vec2> vertices);

  void expand(const glm::vec2 &p);

  std::array<glm::vec2, 4> corners() const;

  glm::vec2 globalPos() const;
};
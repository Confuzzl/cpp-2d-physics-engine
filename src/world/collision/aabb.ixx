export module aabb;

import glm;
import math;
import <vector>;

export struct aabb_t {
  glm::vec2 min{F_INF_POS, F_INF_POS}, max{F_INF_NEG, F_INF_NEG};

  aabb_t() = default;
  aabb_t(const glm::vec2 &min, const glm::vec2 &max);

  bool intersects(const aabb_t &other) const;
  void expand(const glm::vec2 &p);
  void translate(const glm::vec2 &v);

  static aabb_t poly_con(const glm::vec2 &pos, const float rot,
                         const std::vector<glm::vec2> &points);
  static aabb_t circ_con(const glm::vec2 &pos, const float radius);
};

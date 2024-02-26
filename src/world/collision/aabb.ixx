export module aabb;

import glm;
import math;

export struct aabb_t {
private:
  glm::vec2 _min{F_INF_POS, F_INF_POS}, _max{F_INF_NEG, F_INF_NEG};

public:
  glm::vec2 min() const { return _min; }
  glm::vec2 max() const { return _max; }

  aabb_t() = default;
  aabb_t(const glm::vec2 &min, const glm::vec2 &max);

  bool intersects(const aabb_t &other) const;
  void expand(const glm::vec2 &p);
};

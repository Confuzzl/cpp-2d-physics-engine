export module aabb;

import glm;
import math;
import <vector>;

export template <typename vec_t = glm::vec2> struct aabb_t {
  vec_t min{F_INF_POS, F_INF_POS}, max{F_INF_NEG, F_INF_NEG};

  aabb_t() = default;
  aabb_t(const vec_t &min, const vec_t &max) : min{min}, max{max} {}

  template <typename other_vec_t>
  bool intersects(const aabb_t<other_vec_t> &other) const;
  void expand(const vec_t &p);
  void translate(const vec_t &v);

  static aabb_t poly_con(const vec_t &pos, const float rot,
                         const std::vector<vec_t> &points);
  static aabb_t circ_con(const vec_t &pos, const float radius);
};

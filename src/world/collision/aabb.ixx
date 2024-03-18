export module aabb;

import glm;
import math;
import poly;

import <vector>;

export template <typename vec_t = glm::vec2> struct gen_aabb_t {
  using unit_t = vec_t::value_type;

  vec_t min{F_INF_POS, F_INF_POS}, max{F_INF_NEG, F_INF_NEG};

  gen_aabb_t() = default;
  gen_aabb_t(const vec_t &min, const vec_t &max) : min{min}, max{max} {}

  template <typename other_vec_t>
  bool intersects(const gen_aabb_t<other_vec_t> &other) const {
    return (min.x < other.max.x && max.x > other.min.x) &&
           (min.y < other.max.y && max.y > other.min.y);
  }
  template <typename other_vec_t>
  bool contains(const other_vec_t &point) const {
    return in_range(point.x, min.x, max.x) && in_range(point.y, min.y, max.y);
  }
  void expand(const vec_t &p) {
    min.x = std::min(min.x, p.x);
    max.x = std::max(max.x, p.x);
    min.y = std::min(min.y, p.y);
    max.y = std::max(max.y, p.y);
  }
  void expand(const gen_aabb_t &other) {
    expand(other.min);
    expand(other.max);
  }
  void translate(const vec_t &v) {
    min += v;
    max += v;
  }

  unit_t width() const { return max.x - min.x; }
  unit_t height() const { return max.y - min.y; }
  vec_t size() const { return {width(), height()}; }
  unit_t area() const { return width() * height(); }
  vec_t median() const { return min + size() / 2.0f; }

  static gen_aabb_t poly_con(const vec_t &pos, const float rot,
                             const std::vector<vec_t> &points) {
    gen_aabb_t out{};
    for (const glm::vec2 &v : points)
      out.expand(local_to_global(v, pos, rot));
    return out;
  }
  static gen_aabb_t circ_con(const vec_t &pos, const float r) {
    return gen_aabb_t{glm::vec2{-r, -r} + pos, glm::vec2{+r, +r} + pos};
  }
};

export using aabb_t = gen_aabb_t<>;
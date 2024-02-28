module aabb;

import poly;
import debug;

aabb_t::aabb_t(const glm::vec2 &min, const glm::vec2 &max)
    : min{min}, max{max} {}

bool aabb_t::intersects(const aabb_t &other) const {
  return (min.x < other.max.x && max.x > other.min.x) &&
         (min.y < other.max.y && max.y > other.min.y);
}
void aabb_t::expand(const glm::vec2 &p) {
  min.x = std::min(min.x, p.x);
  max.x = std::max(max.x, p.x);
  min.y = std::min(min.y, p.y);
  max.y = std::max(max.y, p.y);
}
void aabb_t::translate(const glm::vec2 &v) {
  min += v;
  max += v;
}

aabb_t aabb_t::poly_con(const glm::vec2 &pos, const float rot,
                        const std::vector<glm::vec2> &points) {
  aabb_t out{};
  for (const glm::vec2 &v : points)
    out.expand(local_to_global(v, pos, rot));
  return out;
}
aabb_t aabb_t::circ_con(const glm::vec2 &pos, const float r) {
  return aabb_t{glm::vec2{-r, -r} + pos, glm::vec2{+r, +r} + pos};
}

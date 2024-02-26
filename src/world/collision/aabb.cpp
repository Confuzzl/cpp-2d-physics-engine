module aabb;

import mesh;

import debug;

aabb_t::aabb_t(const glm::vec2 &min, const glm::vec2 &max)
    : _min{min}, _max{max} {}

bool aabb_t::intersects(const aabb_t &other) const {
  return (_min.x < other._max.x && _max.x > other._min.x) &&
         (_min.y < other._max.y && _max.y > other._min.y);
}
void aabb_t::expand(const glm::vec2 &p) {
  _min.x = std::min(_min.x, p.x);
  _max.x = std::max(_max.x, p.x);
  _min.y = std::min(_min.y, p.y);
  _max.y = std::max(_max.y, p.y);
}

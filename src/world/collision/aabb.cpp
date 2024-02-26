module aabb;

import mesh;

import debug;

aabb_t::aabb_t(const glm::vec2 &min, const glm::vec2 &max)
    : min{min}, max{max} {}

bool aabb_t::intersects(const aabb_t &other) const {
  const glm::vec2 tmin = globalMin(), tmax = globalMax(),
                  omin = other.globalMin(), omax = other.globalMax();
  return (tmin.x < omax.x && tmax.x > omax.x) &&
         (tmin.y < omin.y && tmax.y > omax.y);
}
void aabb_t::expand(const glm::vec2 &p) {
  min.x = std::min(min.x, p.x);
  max.x = std::max(max.x, p.x);
  min.y = std::min(min.y, p.y);
  max.y = std::max(max.y, p.y);
}

module aabb;

import collider;
import mesh;

import debug;

// generates invalid vbo for some reason?
// static VBO<ShapeVertex> sharedVBO() {
//   static VBO<ShapeVertex> out{4};
//   // println("{}", out.ID);
//   return out;
// }

AABB::AABB(const Collider &parent, const glm::vec2 &min, const glm::vec2 &max)
    : parent{parent}, min{min}, max{max},
      vbo{/*sharedVBO()*/ VBOHolder.get(4)} {}
std::unique_ptr<AABB> AABB::from(const Collider &parent, const float radius) {
  return std::make_unique<AABB>(parent, glm::vec2{-radius, -radius},
                                glm::vec2{+radius, +radius});
}
std::unique_ptr<AABB> AABB::from(const Collider &parent,
                                 const std::vector<glm::vec2> vertices) {
  glm::vec2 min{F_INF_POS, F_INF_POS}, max{F_INF_NEG, F_INF_NEG};
  for (const glm::vec2 &v : vertices) {
    min.x = std::min(min.x, v.x);
    max.x = std::max(max.x, v.x);
    min.y = std::min(min.y, v.y);
    max.y = std::max(max.y, v.y);
  }
  return std::make_unique<AABB>(parent, min, max);
}

bool AABB::intersects(const AABB &other) const {
  const glm::vec2 tmin = globalMin(), tmax = globalMax(),
                  omin = other.globalMin(), omax = other.globalMax();
  return (tmin.x < omax.x && tmax.x > omax.x) &&
         (tmin.y < omin.y && tmax.y > omax.y);
}
void AABB::expand(const glm::vec2 &p) {
  min.x = std::min(min.x, p.x);
  max.x = std::max(max.x, p.x);
  min.y = std::min(min.y, p.y);
  max.y = std::max(max.y, p.y);
}

std::array<glm::vec2, 4> AABB::localVertices() const {
  return {{min, {max.x, min.y}, max, {min.x, max.y}}};
}

glm::vec2 AABB::globalPos() const { return parent.pos(); }

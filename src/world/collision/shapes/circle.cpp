module circle;

import aabb;
import polygon;

Circle::Circle(const glm::vec2 &pos, const float r, const float radius)
    : Collider(aabb_t{}, pos, r), radius{radius} {}
// std::unique_ptr<Circle> Circle::create(const Object &parent,
//                                        const float radius) {
//   return std::make_unique<Circle>(parent, radius);
// }
Circle Circle::New(const float radius, const glm::vec2 &pos, const float r) {
  return {pos, r, radius};
}

bool Circle::contains(const glm::vec2 &point) const {
  const float distance2 = glm::distance2(pos(), point);
  const float minDistance2 = radius * radius;
  return distance2 < minDistance2;
}

import polycirc;
import circcirc;

SAT::QueryInfo Circle::reverseQuery(const Collider &other) const {
  return other.query(*this);
}
SAT::QueryInfo Circle::query(const Polygon &other) const {
  return SAT::queryPolyCirc(other, *this, true);
}
SAT::QueryInfo Circle::query(const Circle &other) const {
  return SAT::queryCircCirc(*this, other);
}
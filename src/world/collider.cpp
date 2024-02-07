module;

#include <algorithm>

module collider;

import object;
import poly;

import debug;

Collider::Collider(const Object &parent, std::unique_ptr<AABB> aabb)
    : parent{parent}, aabb{std::move(aabb)} {}

glm::vec2 Collider::globalPos() const { return parent.pos; }

Convex::Convex(const Object &parent, std::vector<glm::vec2> &&vertices)
    : Collider(parent, std::make_unique<AABB>(*this)),
      vertices{std::move(vertices)} {

  std::ranges::for_each(
      this->vertices,
      [&aabb = this->aabb](const glm::vec2 &v) { aabb->expand(v); });
}
std::unique_ptr<Convex> Convex::create(const Object &parent,
                                       const unsigned char n,
                                       const double radius,
                                       const double offset) {
  return std::make_unique<Convex>(parent,
                                  std::move(ngonVertices(n, radius, offset)));
}

Circle::Circle(const Object &parent, const double radius)
    : Collider(parent,
               std::make_unique<AABB>(*this, glm::vec2{-radius, -radius},
                                      glm::vec2{+radius, +radius})),
      radius{radius} {}
std::unique_ptr<Circle> Circle::create(const Object &parent,
                                       const double radius) {
  return std::make_unique<Circle>(parent, radius);
}
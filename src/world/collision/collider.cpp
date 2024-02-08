module;

#include <ranges>

module collider;

import object;
import poly;

import debug;

Collider::Collider(const Object &parent, std::unique_ptr<AABB> aabb)
    : parent{parent}, aabb{std::move(aabb)} {}

const auto &Collider::props() const { return parent.properties; }
glm::vec2 Collider::pos() const { return parent.properties.pos; }
float Collider::rot() const { return parent.properties.rotation; }

AABB &Collider::getAABB() { return *aabb; }

import func;

Convex::Convex(const Object &parent, std::vector<glm::vec2> &&vertices)
    : Collider(parent, std::make_unique<AABB>(*this)),
      vertices{std::move(vertices)},
      vvv{func::map<glm::vec2, vertex>(this->vertices,
                                       [this](const glm::vec2 &v) {
                                         return vertex{*this, v};
                                       })} /*,
       eee{func::map2<vertex, edge>(
           vertices, [this](const vertex &v, const int i ,
                            const std::vector<vertex> &vertices) {
             return edge{*this, v, vertices[(i + 1) % vertices.size()]};
           })}*/
{

  for (const glm::vec2 &v : vertices) {
    aabb->expand(v);
  }
}
std::unique_ptr<Convex> Convex::create(const Object &parent,
                                       const unsigned char n,
                                       const double radius,
                                       const double offset) {
  return std::make_unique<Convex>(parent,
                                  std::move(ngonVertices(n, radius, offset)));
}

auto Convex::globalVertices() const {
  return vertices | std::views::transform([](const glm::vec2 &v) { return v; });
}

Circle::Circle(const Object &parent, const float radius)
    : Collider(parent,
               std::make_unique<AABB>(*this, glm::vec2{-radius, -radius},
                                      glm::vec2{+radius, +radius})),
      radius{radius} {}
std::unique_ptr<Circle> Circle::create(const Object &parent,
                                       const float radius) {
  return std::make_unique<Circle>(parent, radius);
}

import polypoly;
import polycirc;
import circcirc;

SAT::QueryInfo Convex::reverseQuery(const Collider &other) const {
  return other.query(*this);
}
SAT::QueryInfo Convex::query(const Convex &other) const {
  return SAT::queryPolyPoly(*this, other);
}
SAT::QueryInfo Convex::query(const Circle &other) const {
  return SAT::queryPolyCirc(*this, other, false);
}

SAT::QueryInfo Circle::reverseQuery(const Collider &other) const {
  return other.query(*this);
}
SAT::QueryInfo Circle::query(const Convex &other) const {
  return SAT::queryPolyCirc(other, *this, true);
}
SAT::QueryInfo Circle::query(const Circle &other) const {
  return SAT::queryCircCirc(*this, other);
}
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

Polygon::Polygon(const Object &parent, const std::vector<glm::vec2> &points)
    : Collider(parent,
               /*std::make_unique<AABB>(*this)*/ AABB::from(*this, points)),
      vertices{func::map<glm::vec2, vertex>(points,
                                            [this](const glm::vec2 &v) {
                                              return vertex{*this, v};
                                            })},
      edges{func::map<vertex, edge>(
          vertices, [this](const vertex &v, const int i,
                           const std::vector<vertex> &vertices) {
            return edge{*this, v, vertices[(i + 1) % vertices.size()]};
          })} {}
std::unique_ptr<Polygon> Polygon::create(const Object &parent,
                                         const unsigned char n,
                                         const double radius,
                                         const double offset) {
  return std::make_unique<Polygon>(parent,
                                   std::move(ngonVertices(n, radius, offset)));
}

const std::vector<Polygon::vertex> &Polygon::getVertices() const {
  return vertices;
}
auto Polygon::localVertices() const {
  return vertices |
         std::views::transform([](const vertex &v) { return v.localPos(); });
}
auto Polygon::globalVertices() const {
  return vertices |
         std::views::transform([](const vertex &v) { return v.globalPos(); });
}

Circle::Circle(const Object &parent, const float radius)
    : Collider(parent, /*
                std::make_unique<AABB>(*this, glm::vec2{-radius, -radius},
                                       glm::vec2{+radius, +radius})*/
               AABB::from(*this, radius)),
      radius{radius} {}
std::unique_ptr<Circle> Circle::create(const Object &parent,
                                       const float radius) {
  return std::make_unique<Circle>(parent, radius);
}

import polypoly;
import polycirc;
import circcirc;

SAT::QueryInfo Polygon::reverseQuery(const Collider &other) const {
  return other.query(*this);
}
SAT::QueryInfo Polygon::query(const Polygon &other) const {
  return SAT::queryPolyPoly(*this, other);
}
SAT::QueryInfo Polygon::query(const Circle &other) const {
  return SAT::queryPolyCirc(*this, other, false);
}

SAT::QueryInfo Circle::reverseQuery(const Collider &other) const {
  return other.query(*this);
}
SAT::QueryInfo Circle::query(const Polygon &other) const {
  return SAT::queryPolyCirc(other, *this, true);
}
SAT::QueryInfo Circle::query(const Circle &other) const {
  return SAT::queryCircCirc(*this, other);
}
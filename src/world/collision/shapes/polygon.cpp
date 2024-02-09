module polygon;

import func;
import circle;

Polygon::Polygon(const Object &parent, const std::vector<glm::vec2> &points)
    : Collider(parent,
               /*std::make_unique<AABB>(*this)*/ AABB::from(*this, points)),
      vertices{func::map<glm::vec2, vertex>(points,
                                            [this](const glm::vec2 &v) {
                                              return vertex{*this, v};
                                            })},
      edges{func::map<vertex, edge>(
          vertices,
          [this](const vertex &v, const int i,
                 const std::vector<vertex> &vertices) {
            return edge{*this, v, vertices[(i + 1) % vertices.size()]};
          })},
      sides{static_cast<unsigned char>(points.size())} {}
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
// auto Polygon::localVertices() const {
//   return vertices |
//          std::views::transform([](const vertex &v) { return v.localPos(); });
// }
// auto Polygon::globalVertices() const {
//   return vertices |
//          std::views::transform([](const vertex &v) { return v.globalPos();
//          });
// }

const std::vector<Polygon::edge> &Polygon::getEdges() const { return edges; }

import polypoly;
import polycirc;

SAT::QueryInfo Polygon::reverseQuery(const Collider &other) const {
  return other.query(*this);
}
SAT::QueryInfo Polygon::query(const Polygon &other) const {
  return SAT::queryPolyPoly(*this, other);
}
SAT::QueryInfo Polygon::query(const Circle &other) const {
  return SAT::queryPolyCirc(*this, other, false);
}
module polygon;

import func;
import circle;

Polygon::Polygon(const glm::vec2 &pos, const float r,
                 std::vector<glm::vec2> &&points)
    : Collider(aabb_t{}, pos, r),
      vertices{func::map<glm::vec2, vertex_t>(points,
                                              [this](const glm::vec2 &v) {
                                                return vertex_t{*this, v};
                                              })},
      edges{func::map<vertex_t, edge_t>(
          vertices,
          [this](const vertex_t &v, const int i,
                 const std::vector<vertex_t> &vertices) {
            return edge_t{*this, v, vertices[(i + 1) % vertices.size()]};
          })},
      sides{static_cast<unsigned char>(points.size())} {}

Polygon Polygon::New(const opts_t &opts, const glm::vec2 pos, const float r) {
  return {pos, r, ngonVertices(opts., radius, offset)};
}

// std::unique_ptr<Polygon> Polygon::create(const Object &parent,
//                                          const unsigned char n,
//                                          const double radius,
//                                          const double offset) {
//   return std::make_unique<Polygon>(parent,
//                                    std::move(ngonVertices(n, radius,
//                                    offset)));
// }

// const std::vector<Polygon::vertex_t> &Polygon::getVertices() const {
//   return vertices;
// }
//  auto Polygon::localVertices() const {
//    return vertices |
//           std::views::transform([](const vertex &v) { return v.localPos();
//           });
//  }
//  auto Polygon::globalVertices() const {
//    return vertices |
//           std::views::transform([](const vertex &v) { return v.globalPos();
//           });
//  }

const std::vector<Polygon::edge_t> &Polygon::getEdges() const { return edges; }

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
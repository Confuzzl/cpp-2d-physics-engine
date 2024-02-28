module polygon;

import func;
import circle;

Polygon::Polygon(const glm::vec2 &pos, const float r,
                 std::vector<glm::vec2> &&points)
    : Collider(aabb_t::poly_con(pos, r, points), pos, r),
      sides{static_cast<unsigned char>(points.size())},
      vertices{std::move(points)},
      edges{func::map<vertex_t, edge_t>(
          vertices,
          [this](const vertex_t &v, const int i,
                 const std::vector<vertex_t> &vertices) {
            return edge_t{
                this, static_cast<unsigned char>(i),
                static_cast<unsigned char>((i + 1) % vertices.size())};
          })},
      globalVertexView{newVerticesView()} {}

Polygon Polygon::New(const opts_t &opts, const glm::vec2 pos, const float r) {
  return {pos, r, ngonVertices(opts.n, opts.r, opts.offset)};
}

void Polygon::translateAABB(const glm::vec2 &offset) {
  getAABB().translate(offset);
  updateGlobalVertexView();
}
void Polygon::refreshAABB() {
  getAABB() = aabb_t::poly_con(pos(), rot(), vertices);
}

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
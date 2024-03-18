module;

#include <algorithm>

export module polypoly;

import polygon;
import query_info;
import axis;
import vector;

import debug;
import glm;

export namespace SAT {
struct depth_info_t {
  const Polygon::edge_t *edge;
  axis_t axis;
};

enum struct PROJECTION_STATE { NONE, INTERSECTION };
PROJECTION_STATE projectToDepths(const Polygon &a, const Polygon &b,
                                 std::vector<depth_info_t> &depths) {
  for (const Polygon::edge_t &edge : a.getEdges()) {
    axis_t axis{edge.getNormal()};
    axis.projectPolygons(a, b);
    if (!axis.isIntersecting())
      return PROJECTION_STATE::NONE;
    depths.emplace_back(depth_info_t{.edge = &edge, .axis = std::move(axis)});
  }
  return PROJECTION_STATE::INTERSECTION;
}

QueryInfo queryPolyPoly(const Polygon &a, const Polygon &b) {
  std::vector<depth_info_t> depths{};
  depths.reserve(a.sides + b.sides);

  const PROJECTION_STATE ab = projectToDepths(a, b, depths);
  if (ab == PROJECTION_STATE::NONE)
    return QueryInfo::NONE();
  const PROJECTION_STATE ba = projectToDepths(b, a, depths);
  if (ba == PROJECTION_STATE::NONE)
    return QueryInfo::NONE();

  std::ranges::sort(
      depths, &depth_func::compare,
      [](const depth_info_t &depth) { return depth.axis.depth(); });
  const depth_info_t &best = depths[0];
  return QueryInfo{.collision = true,
                   .pointA = {},
                   .pointB = {},
                   .normalA = best.edge->parent() == &a
                                  ? best.edge->getNormal()
                                  : best.edge->getNormal() * -1.0f};
}

} // namespace SAT
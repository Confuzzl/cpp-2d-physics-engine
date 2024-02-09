module;

#include <algorithm>

export module polypoly;

import polygon;
import query_info;
import axis;
import vector;

export namespace SAT {
struct depth_info_t {
  const Polygon::edge &edge;
  Axis axis;
};

enum struct PROJECTION_STATE { NONE, INTERSECTION };
PROJECTION_STATE projectToDepths(const Polygon &a, const Polygon &b,
                                 std::vector<depth_info_t> &depths) {
  for (const Polygon::edge &edge : a.getEdges()) {
    Axis axis{edge.getNormal()};
    axis.projectPolygons(a, b);
    if (!axis.isIntersecting())
      return PROJECTION_STATE::NONE;
    depths.emplace_back(depth_info_t{.edge = edge, .axis = std::move(axis)});
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

  // std::ranges::sort(depths, depth_func::compare, [](const depth_info_t
  // &depth) {
  //   return depth.axis.depth();
  // });

  return QueryInfo::NONE();
}

} // namespace SAT
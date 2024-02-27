export module polycirc;

import polygon;
import circle;
import query_info;
import sat_misc;
import glm;

export namespace SAT {
QueryInfo queryPolyCirc(const Polygon &polygon, const Circle &circle,
                        const bool reverse) {
  QueryInfo out{};
  for (const Polygon::edge_t &edge : polygon.getEdges()) {
    const auto info = edgeCircleQuery(edge, circle);
    if (!(circle.contains(edge.tail()) || circle.contains(edge.head()))) {
      if (!edge.contains(info.edgePoint))
        continue;
      const float distance2 = glm::distance2(info.edgePoint, circle.pos());
      if (distance2 > circle.radius * circle.radius)
        continue;
    }

    out.collision = true;
    if (!depth_func::compare(info.depth, out.depth))
      continue;
    out.depth = info.depth;
    out.setPoints(info.edgePoint, info.circlePoint, reverse);
    out.calculateNormal();
  }
  for (const glm::vec2 &vertex : polygon.globalVertices()) {
    const glm::vec2 diff = vertex - circle.pos();
    const float distance = glm::length(diff);
    const float depth = distance - circle.radius;
    if (depth > 0)
      continue;

    out.collision = true;
    if (!depth_func::compare(depth, out.depth))
      continue;
    out.depth = depth;
    const glm::vec2 circlePoint =
        circle.pos() + glm::normalize(diff) * circle.radius;
    out.setPoints(vertex, circlePoint, reverse);
    out.calculateNormal();
  }
  return QueryInfo::NONE();
}
} // namespace SAT
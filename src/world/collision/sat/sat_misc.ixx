export module sat_misc;

import glm;
import polygon;
import circle;
import math;

export namespace SAT {
struct line_point_query_t {
  const glm::vec2 point;
  const float signedDistance;
};
line_point_query_t linePointQuery(const glm::vec2 &linePoint,
                                  const glm::vec2 &lineDirection,
                                  const glm::vec2 &normal,
                                  const glm::vec2 &point) {
  const glm::vec2 diff = point - linePoint;
  const glm::vec2 direction = glm::normalize(lineDirection);
  const float dot = glm::dot(diff, direction);
  return {.point = linePoint + direction * dot,
          .signedDistance = glm::dot(diff, normal)};
}
struct edge_circle_query_t {
  const glm::vec2 edgePoint;
  const glm::vec2 circlePoint;
  const float depth;
};
edge_circle_query_t edgeCircleQuery(const Polygon::edge &edge,
                                    const Circle &circle) {
  const auto &[point, distance] =
      linePointQuery(edge.globalTail(), edge, edge.getNormal(), circle.pos());
  const float depth = signum(distance) * (std::abs(distance) - circle.radius);
  return {.edgePoint = point,
          .circlePoint = point + edge.getNormal() * depth,
          .depth = depth};
};
} // namespace SAT
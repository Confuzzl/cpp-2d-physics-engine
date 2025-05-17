module shapes;

import math;
import sat;

namespace collision {
namespace SAT {
using AxisProjFunc = void (Axis::*)(const glm::vec2);

void projectToAxis(Axis &axis, AxisProjFunc func, const Polygon &shape) {
  for (const auto v : shape.getVertices())
    (axis.*func)(v);
}
void projectToAxis(Axis &axis, AxisProjFunc func, const Circle &shape) {
  (axis.*func)(shape.center + axis.direction * shape.radius);
  (axis.*func)(shape.center - axis.direction * shape.radius);
}
void projectToAxis(Axis &axis, AxisProjFunc func, const Wall &shape) {
  const auto [a, b] = shape.vertices();
  (axis.*func)(a);
  (axis.*func)(b);
}

template <typename T, typename... D>
bool projectToPolygon(const Polygon &a, const T &b, D &...opt) {
  static_assert(sizeof...(D) <= 1);

  for (const auto edge : a.edges()) {
    const auto n = edge.normal();
    Axis axis{n};
    projectToAxis(axis, &Axis::projectA, a);
    projectToAxis(axis, &Axis::projectB, b);
    if (!axis.intersecting())
      return false;
    if constexpr (sizeof...(D) == 1) {
      DepthInfo &min = (opt, ...);
      if (axis.depth() < min.depth())
        min = {.poly = &a, .normal = n, .axis = axis};
    }
  }
  return true;
}

template <typename... D>
bool resolveImpl(const Polygon &a, const Polygon &b, D &...opt) {
  return projectToPolygon(a, b, opt...) && projectToPolygon(b, a, opt...);
}
template <typename... D>
bool resolveImpl(const Polygon &a, const Circle &b, D &...opt) {
  const bool edges = projectToPolygon(a, b, opt...);
  // avoid vertex check
  if (!edges)
    return false;

  glm::vec2 minVertex = a.getVertices()[0];
  float minDist = glm::distance2(b.center, minVertex);
  // use derivative to end vertex check early
  for (const auto v : a.getVertices()) {
    const float d = glm::distance2(b.center, v);
    if (d > minDist)
      continue;
    minVertex = v;
    minDist = d;
  }

  Axis axis{b.center - minVertex};
  projectToAxis(axis, &Axis::projectA, a);
  projectToAxis(axis, &Axis::projectB, b);
  return axis.intersecting();
}
template <typename... D>
bool resolveImpl(const Polygon &a, const Wall &b, D &...opt) {
  static_assert(sizeof...(D) <= 1);

  const auto n = b.normal();
  Axis axis{n};
  projectToAxis(axis, &Axis::projectA, a);
  projectToAxis(axis, &Axis::projectB, b);
  if constexpr (sizeof...(D) == 1) {
    DepthInfo &min = (opt, ...);
    if (axis.depth() < min.depth())
      min = {.poly = &a, .normal = n, .axis = axis};
  }
  // println("A {} {} | B {} {} | {}", axis.minA, axis.maxA, axis.minB,
  // axis.maxB,
  //         axis.depth());
  return axis.intersecting();
}

template <typename A, typename B>
CollisionData resolve(const A &a, const B &b) {
  DepthInfo best{.axis{INF}};

  if (!resolveImpl(a, b, best))
    return {};

  const glm::vec2 direction = (best.poly == &a ? -1.0f : +1.0f) * best.normal;
  // println("resolve {} {}", vec_string(best.normal), best.depth());
  return {.a{}, .b = direction * best.depth()};
}
template <typename A, typename B> bool colliding(const A &a, const B &b) {
  return resolveImpl(a, b);
}
} // namespace SAT

template <> CollisionData queryCollision(const Wall &a, const Wall &b) {
  throw std::runtime_error{std::format("TODO {}", __func__)};
  // return {};
}
template <> CollisionData queryCollision(const Wall &a, const Circle &b) {
  // https://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment
  const auto [v1, v2] = a.vertices();
  const auto normal = a.normal();

  const auto cpos = b.center;

  const bool behind = glm::dot(normal, v1 - cpos) > 0;
  if (!a.bidirectional && behind)
    return {};

  const auto line = v2 - v1;
  const float t = std::max(
      0.0f, std::min(glm::dot(cpos - v1, line) / glm::length2(line), 1.0f));

  const auto proj = v1 + t * line;
  const auto diff = cpos - proj;
  if (glm::length2(diff) > b.radius * b.radius)
    return {};

  return {.a = proj, .b = cpos - (behind ? -1.f : 1.f) * normal * b.radius};
}
template <> CollisionData queryCollision(const Wall &a, const Polygon &b) {
  const auto [v1, v2] = a.vertices();
  const auto normal = a.normal();

  const bool behind = glm::dot(normal, v1 - b.getCenter()) > 0;
  if (!a.bidirectional && behind)
    return {};

  const CollisionData fo = SAT::resolve(b, a);
  return behind ? fo.reverse() : fo;
}
template <> CollisionData queryCollision(const Circle &a, const Circle &b) {
  const auto p1 = a.center, p2 = b.center;

  const auto rsum = a.radius + b.radius;
  const auto diff = p2 - p1;
  if (glm::dot(diff, diff) > rsum * rsum)
    return {};
  const auto diffN =
      (diff.x == 0 && diff.y == 0) ? glm::vec2{1, 0} : glm::normalize(diff);
  return {.a = p1 + diffN * a.radius, .b = p2 - diffN * b.radius};
}
template <> CollisionData queryCollision(const Circle &a, const Polygon &b) {
  return SAT::resolve(b, a);
  // throw std::runtime_error{std::format("TODO {}", __func__)};
}
template <> CollisionData queryCollision(const Polygon &a, const Polygon &b) {
  // throw std::runtime_error{std::format("TODO {}", __func__)};
  return SAT::resolve(a, b);
}
} // namespace collision

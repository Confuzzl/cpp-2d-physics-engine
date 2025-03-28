export module shapes;

import glm;
import runtime_array;
import <variant>;
import <ranges>;
import aabb;

import debug;

template <typename T> static auto transformWith(const T &transform) {
  return std::views::transform([&transform](const glm::vec2 p) {
    return ::transform(p, transform.position, transform.rotation);
  });
}

export namespace collision {
struct CollisionData {
  glm::vec2 a, b;

  glm::vec2 vec() const { return b - a; }
  glm::vec2 normal() const { return glm::normalize(vec()); }
  float length() const { return glm::length(vec()); }
  operator bool() const { return vec() != glm::vec2{0}; }
};

using Box = BoundingBox;
struct Wall {
private:
  glm::vec2 a, b;
  glm::vec2 _normal;

public:
  bool bidirectional;

  Wall(const glm::vec2 a, const glm::vec2 b, const glm::vec2 normal,
       const bool bidirectional)
      : a{a}, b{b}, _normal{normal}, bidirectional{bidirectional} {}
  Wall(const glm::vec2 a, const glm::vec2 b, const bool bidirectional = false)
      : Wall(a, b, glm::normalize(cw_perp(b - a)), bidirectional) {}

  template <typename T>
  std::pair<glm::vec2, glm::vec2> getVertices(const T &transform) const {
    return {::transform(a, transform.position, transform.rotation),
            ::transform(b, transform.position, transform.rotation)};
  }
  // template <typename T> auto vertexTransform(const T &transform) const {
  //   return vertices | transformWith(transform);
  // }
  template <typename T> glm::vec2 normal(const T &transform) const {
    return ::transform(_normal, {}, transform.rotation);
  }
};
struct Circle {
  float radius;
};
struct Polygon {
  runtime_array<glm::vec2> vertices;
};
struct Shape {
  std::variant<Wall, Circle, Polygon> data;

  template <typename T> Shape(T &&shape) : data{std::forward<T>(shape)} {}

  template <typename T> bool is() const {
    return std::holds_alternative<T>(data);
  }
  template <typename T> T &to() { return std::get<T>(data); }
  template <typename T> const T &to() const { return std::get<T>(data); }

  template <typename W, typename C, typename P>
  auto visit(W &&w, C &&c, P &&p) const {
    if (is<Wall>())
      return w(to<Wall>());
    if (is<Circle>())
      return c(to<Circle>());
    return p(to<Polygon>());
  }
};

template <typename T>
CollisionData push(const Wall &wall, const T &ta, const Circle &circle,
                   const T &tb) {
  // https://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment
  const glm::vec2 p1 = ta.position, p2 = tb.position;

  const auto [i, j] = wall.getVertices(ta);
  const auto a = i + p1, b = j + p1;

  if (!wall.bidirectional) {
    if (glm::dot(wall.normal(ta), a - p2) > 0)
      return {};
  }

  const auto line = b - a;
  const float t = std::max(
      0.0f, std::min(glm::dot(p2 - a, line) / glm::length2(line), 1.0f));
  // const auto proj = (1 - t) * a + t * b;
  const auto proj = a + t * line;
  const auto diff = p2 - proj;
  const float d2 = glm::length2(diff);
  if (d2 > circle.radius * circle.radius)
    return {};

  const auto diffN = glm::normalize(diff);
  return {.a = proj, .b = p2 - diffN * circle.radius};
}
template <typename T>
CollisionData push(const Circle &c1, const T &ta, const Circle &c2,
                   const T &tb) {
  const glm::vec2 p1 = ta.position, p2 = tb.position;

  const auto rsum = c1.radius + c2.radius;
  const auto diff = p2 - p1;
  if (glm::dot(diff, diff) > rsum * rsum)
    return {};
  const auto diffN = glm::normalize(diff);
  return {.a = p1 + diffN * c1.radius, .b = p2 - diffN * c2.radius};
}

} // namespace collision
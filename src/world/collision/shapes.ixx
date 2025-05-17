export module shapes;

import glm;
import runtime_array;
import <variant>;
import <ranges>;
import aabb;

import debug;
import dispatcher;

export namespace collision {
struct CollisionData {
  glm::vec2 a, b;

  glm::vec2 vec() const { return b - a; }
  glm::vec2 normal() const { return glm::normalize(vec()); }
  float length() const { return glm::length(vec()); }
  operator bool() const { return /*vec() != glm::vec2{0}*/ a != b; }

  CollisionData reverse() const { return {b, a}; }
};

struct Wall;
struct Circle;
struct Polygon;

struct Shape {
private:
  TypeSizeType type;

protected:
  template <typename T> Shape(const T *);

public:
  ~Shape() = default;

  TypeSizeType getType() const { return type; }

  template <typename T> bool is() const;
  template <typename W, typename C, typename P>
  auto visit(W &&w, C &&c, P &&p) const {
    if (is<Wall>())
      return w(static_cast<const Wall &>(*this));
    if (is<Circle>())
      return c(static_cast<const Circle &>(*this));
    return p(static_cast<const Polygon &>(*this));
  }

  virtual BoundingBox calcBox(const glm::vec2, const float) const = 0;
};

struct Wall : Shape {
private:
  glm::vec2 a, b;
  glm::vec2 _normal;

public:
  bool bidirectional;

  Wall(const glm::vec2 a, const glm::vec2 b, const glm::vec2 normal,
       const bool bidirectional)
      : Shape(this), a{a}, b{b}, _normal{normal}, bidirectional{bidirectional} {
  }
  Wall(const glm::vec2 a, const glm::vec2 b, const bool bidirectional = false)
      : Wall(a, b, glm::normalize(cw_perp(b - a)), bidirectional) {}

  std::pair<glm::vec2, glm::vec2> vertices() const { return {a, b}; }
  glm::vec2 normal() const { return _normal; }

  Wall transform(const glm::vec2 pos, const float rot) const {
    return {::transform(a, pos, rot), ::transform(b, pos, rot), bidirectional};
  }
  Wall translate(const glm::vec2 pos) const {
    return {a + pos, b + pos, bidirectional};
  }
  BoundingBox calcBox() const { return BoundingBox::checked(a, b); }
  BoundingBox calcBox(const glm::vec2 pos, const float rot) const override {
    return transform(pos, rot).calcBox();
  }
};
struct Circle : Shape {
  float radius;
  glm::vec2 center;

  Circle(const float radius, const glm::vec2 center = {})
      : Shape(this), radius{radius}, center{center} {}

  Circle transform(const glm::vec2 pos, const float rot) const {
    return {radius, ::transform(center, pos, rot)};
  }
  Circle translate(const glm::vec2 pos) const { return {radius, center + pos}; }
  BoundingBox calcBox() const { return BoundingBox{radius} + center; }
  BoundingBox calcBox(const glm::vec2 pos, const float rot) const override {
    return transform(pos, rot).calcBox();
  }
};
struct Polygon : Shape {
  using VertexArray = runtime_array<glm::vec2>;

private:
  glm::vec2 center;
  VertexArray vertices;

public:
  struct Edge {
    glm::vec2 a, b;

    glm::vec2 normal() const { return glm::normalize(cw_perp(b - a)); }
  };
  struct EdgeView {
    const VertexArray *vertices;

    struct iterator {
      const VertexArray *vertices;
      std::size_t index;

      iterator operator++() {
        index++;
        return *this;
      }
      bool operator!=(const iterator &that) const {
        return index != that.index;
      }
      Edge operator*() const {
        return {.a = (*vertices)[index],
                .b = (*vertices)[(index + 1) % vertices->size()]};
      }
    };
    iterator begin() const { return {.vertices = vertices, .index = 0}; }
    iterator end() const {
      return {.vertices = vertices, .index = vertices->size()};
    }
    auto size() const { return vertices->size(); }
  };

  Polygon() : Shape(this) {}
  Polygon(std::initializer_list<glm::vec2> vertices)
      : Shape(this), vertices{vertices} {}

  glm::vec2 getCenter() const { return center; }
  const VertexArray &getVertices() const { return vertices; }
  EdgeView edges() const { return {.vertices = &vertices}; }

  Polygon transform(const glm::vec2 pos, const float rot) const {
    Polygon out{};
    out.center = pos;
    out.vertices = vertices;
    for (glm::vec2 &v : out.vertices)
      v = ::transform(v, pos, rot);
    return out;
  }
  Polygon translate(const glm::vec2 pos) const {
    Polygon out{};
    out.center = pos;
    out.vertices = vertices;
    for (glm::vec2 &v : out.vertices)
      v += pos;
    return out;
  }
  BoundingBox calcBox() const {
    BoundingBox out;
    for (const auto v : vertices)
      out.expand(v);
    return out;
  }
  BoundingBox calcBox(const glm::vec2 pos, const float rot) const override {
    return transform(pos, rot).calcBox();
  }

  static Polygon box(const glm::vec2 size) {
    const glm::vec2 half = size / 2.0f;
    return {
        {-half.x, -half.y},
        {+half.x, -half.y},
        {+half.x, +half.y},
        {-half.x, +half.y},
    };
  }
};

using ShapeTypes = TypeRegistrar<Wall, Circle, Polygon>;
template <typename T>
Shape::Shape(const T *) : type{ShapeTypes::getIndex<T>()} {}
template <typename T> bool Shape::is() const {
  return type == ShapeTypes::getIndex<T>();
}

template <typename A, typename B>
CollisionData queryCollision(const A &a, const B &b) {
  return queryCollision(b, a).reverse();
}
template <> CollisionData queryCollision(const Wall &a, const Wall &b);
template <> CollisionData queryCollision(const Wall &a, const Circle &b);
template <> CollisionData queryCollision(const Wall &a, const Polygon &b);
template <> CollisionData queryCollision(const Circle &a, const Circle &b);
template <> CollisionData queryCollision(const Circle &a, const Polygon &b);
template <> CollisionData queryCollision(const Polygon &a, const Polygon &b);

} // namespace collision

export template <> TypeSizeType getType(const collision::Shape &s) {
  return s.getType();
}
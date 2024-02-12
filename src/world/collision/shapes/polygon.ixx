module;

#include <ranges>

export module polygon;

import collider;
import glm;
import poly;
import vector;
import query_info;

export struct Circle;

export struct Polygon : public Collider {
  struct vertex {
    const Polygon &parent;

  private:
    const glm::vec2 pos;

  public:
    vertex(const Polygon &parent, const glm::vec2 &pos)
        : parent{parent}, pos{pos} {}

    const glm::vec2 &localPos() const { return pos; }
    glm::vec2 globalPos() const {
      return vertexLocalToGlobal(pos, parent.pos(), parent.rot());
    }
  };

  struct edge {
    const Polygon &parent;

  private:
    const vertex &tail, &head;
    const glm::vec2 normal;

    glm::vec2 calculateNormal() {
      const glm::vec3 tail3{tail.localPos(), 0};
      const glm::vec3 head3{head.localPos(), 0};
      const glm::vec3 up{glm::cross(tail3, head3)};
      const glm::vec3 this3{static_cast<glm::vec2>(*this), 0};
      const glm::vec3 norm3{glm::cross(this3, up)};
      return glm::normalize(glm::vec2{norm3.x, norm3.y});
    }

  public:
    edge(const Polygon &parent, const vertex &tail, const vertex &head)
        : parent{parent}, tail{tail}, head{head}, normal{calculateNormal()} {}

    glm::vec2 localTail() const { return tail.localPos(); }
    glm::vec2 localHead() const { return head.localPos(); }
    glm::vec2 globalTail() const { return localTail() + parent.pos(); }
    glm::vec2 globalHead() const { return localHead() + parent.pos(); }
    const glm::vec2 &getNormal() const { return normal; }

    operator glm::vec2() const { return head.localPos() - tail.localPos(); }

    bool contains(const glm::vec2 &point) const {
      return glm::distance(globalTail(), point) +
                 glm::distance(point, globalHead()) ==
             glm::length(static_cast<glm::vec2>(*this));
    }
  };

private:
  const std::vector<vertex> vertices;
  const std::vector<edge> edges;

  friend std::unique_ptr<Polygon>
  std::make_unique<Polygon, const Object &, std::vector<glm::vec2>>(
      const Object &, std::vector<glm::vec2> &&);
  Polygon(const Object &parent, const std::vector<glm::vec2> &points);

public:
  const unsigned char sides;

  static std::unique_ptr<Polygon> create(const Object &parent,
                                         const unsigned char n,
                                         const double radius,
                                         const double offset);

  const std::vector<vertex> &getVertices() const;
  auto localVertices() const {
    return vertices |
           std::views::transform([](const vertex &v) { return v.localPos(); });
  }
  auto globalVertices() const {
    return vertices |
           std::views::transform([](const vertex &v) { return v.globalPos(); });
  }

  const std::vector<edge> &getEdges() const;

  SAT::QueryInfo reverseQuery(const Collider &other) const override;
  SAT::QueryInfo query(const Polygon &other) const override;
  SAT::QueryInfo query(const Circle &other) const override;
};
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
  struct vertex_t {
    const Polygon &parent;

  private:
    const glm::vec2 pos;

  public:
    vertex_t(const Polygon &parent, const glm::vec2 &pos)
        : parent{parent}, pos{pos} {}

    const glm::vec2 &localPos() const { return pos; }
    glm::vec2 globalPos() const {
      return vertexLocalToGlobal(pos, parent.pos(), parent.rot());
    }
  };

  struct edge_t {
    const Polygon &parent;

  private:
    const vertex_t &tail, &head;

  public:
    edge_t(const Polygon &parent, const vertex_t &tail, const vertex_t &head)
        : parent{parent}, tail{tail}, head{head} {}

    glm::vec2 localTail() const { return tail.localPos(); }
    glm::vec2 localHead() const { return head.localPos(); }
    glm::vec2 globalTail() const { return tail.globalPos(); }
    glm::vec2 globalHead() const { return head.globalPos(); }
    glm::vec2 getNormal() const { return glm::normalize(cwPerp(*this)); }

    operator glm::vec2() const { return head.globalPos() - tail.globalPos(); }

    bool contains(const glm::vec2 &point) const {
      return glm::distance(globalTail(), point) +
                 glm::distance(point, globalHead()) ==
             glm::length(static_cast<glm::vec2>(*this));
    }
  };

private:
  const std::vector<vertex_t> vertices;
  const std::vector<edge_t> edges;

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

  const std::vector<vertex_t> &getVertices() const;
  auto localVertices() const {
    return vertices | std::views::transform(
                          [](const vertex_t &v) { return v.localPos(); });
  }
  auto globalVertices() const {
    return vertices | std::views::transform(
                          [](const vertex_t &v) { return v.globalPos(); });
  }

  const std::vector<edge_t> &getEdges() const;

  SAT::QueryInfo reverseQuery(const Collider &other) const override;
  SAT::QueryInfo query(const Polygon &other) const override;
  SAT::QueryInfo query(const Circle &other) const override;
};
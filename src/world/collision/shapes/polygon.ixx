module;

#include <ranges>

export module polygon;

import collider;
import glm;
import poly;
import vector;
import query_info;

import <functional>;

export struct Circle;

export struct Polygon : public Collider {
  using vertex_t = glm::vec2;

  struct edge_t {
  private:
    const Polygon *_parent;

    unsigned char tailIndex, headIndex;

  public:
    edge_t(const Polygon *parent, const unsigned char tailIndex,
           const unsigned char headIndex)
        : _parent{parent}, tailIndex{tailIndex}, headIndex{headIndex} {}

    const Polygon *parent() const { return _parent; }

    glm::vec2 tail() const { return _parent->globalVertices()[tailIndex]; }
    glm::vec2 head() const { return _parent->globalVertices()[headIndex]; }
    glm::vec2 getNormal() const {
      return glm::normalize(cwPerp(static_cast<glm::vec2>(*this)));
    }

    explicit operator glm::vec2() const { return head() - tail(); }

    bool contains(const glm::vec2 &point) const {
      return glm::distance(tail(), point) + glm::distance(point, head()) ==
             glm::length(static_cast<glm::vec2>(*this));
    }
  };

  const unsigned char sides;

private:
  std::vector<vertex_t> vertices;
  std::vector<edge_t> edges;

public:
  Polygon(const glm::vec2 &pos, const float r, std::vector<glm::vec2> &&points);

  struct opts_t {
    unsigned char n;
    float r = 1;
    float offset = 0;
  };
  static Polygon New(const opts_t &opts, const glm::vec2 pos = {0, 0},
                     const float r = 0);

private:
  using global_view_t = std::ranges::transform_view<
      std::ranges::ref_view<const std::vector<vertex_t>>,
      std::function<glm::vec2(const vertex_t &)>>;
  global_view_t newVerticesView() const {
    const glm::vec2 offset = pos();
    return vertices |
           std::views::transform(std::function<glm::vec2(const vertex_t &)>{
               [offset](const vertex_t &v) { return v + offset; }});
  }
  global_view_t globalVertexView;
  void updateGlobalVertexView() { globalVertexView = newVerticesView(); }

public:
  const global_view_t &globalVertices() const { return globalVertexView; }

  void translateAABB(const glm::vec2 &offset) override;
  void refreshAABB() override;

  const std::vector<edge_t> &getEdges() const;

  SAT::QueryInfo reverseQuery(const Collider &other) const override;
  SAT::QueryInfo query(const Polygon &other) const override;
  SAT::QueryInfo query(const Circle &other) const override;
};

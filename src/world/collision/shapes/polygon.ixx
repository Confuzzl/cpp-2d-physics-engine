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
  private:
    glm::vec2 _pos;

  public:
    vertex_t(const glm::vec2 &pos) : _pos{pos} {}

    glm::vec2 pos() const { return _pos; }
  };

  struct edge_t {
  private:
    const std::vector<glm::vec2> &vertices;
    const unsigned char tailIndex, headIndex;

  public:
    edge_t(const std::vector<glm::vec2> &vertices,
           const unsigned char tailIndex, const unsigned char headIndex)
        : vertices{vertices}, tailIndex{tailIndex}, headIndex{headIndex} {}

    glm::vec2 tail() const { return; }
    glm::vec2 head() const { return; }
    glm::vec2 getNormal() const {
      return glm::normalize(cwPerp(static_cast<glm::vec2>(*this)));
    }

    explicit operator glm::vec2() const {
      return vertices[headIndex] - vertices[tailIndex];
    }

    bool contains(const glm::vec2 &point) const {
      return glm::distance(tail(), point) + glm::distance(point, head()) ==
             glm::length(static_cast<glm::vec2>(*this));
    }
  };

  // private:
  std::vector<vertex_t> vertices;
  std::vector<edge_t> edges;

  // friend std::unique_ptr<Polygon>
  // std::make_unique<Polygon, const Object &, std::vector<glm::vec2>>(
  //     const Object &, std::vector<glm::vec2> &&);

  Polygon(const glm::vec2 &pos, const float r, std::vector<glm::vec2> &&points);

  struct opts_t {
    unsigned char n;
    float r = 1;
    float offset = 0;
  };
  static Polygon New(const opts_t &opts, const glm::vec2 pos = {0, 0},
                     const float r);

  // public:
  //   const unsigned char sides;
  //
  //   static std::unique_ptr<Polygon> create(const Object &parent,
  //                                          const unsigned char n,
  //                                          const double radius,
  //                                          const double offset);

  // const std::vector<vertex_t> &getVertices() const;
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
export module collider;

import glm;

export struct Object;

import vector;
import <optional>;
import <memory>;

// export struct AABB;
import aabb;
// not move constuctible if using incomplete uptr

import poly;
import query_info;

struct Polygon;
struct Circle;
export struct Collider {
  const Object &parent;

protected:
  std::unique_ptr<AABB> aabb;
  Collider(const Object &parent, std::unique_ptr<AABB> aabb);

public:
  const auto &props() const;
  glm::vec2 pos() const;
  float rot() const;

  AABB &getAABB();

  virtual SAT::QueryInfo reverseQuery(const Collider &other) const = 0;
  virtual SAT::QueryInfo query(const Polygon &other) const = 0;
  virtual SAT::QueryInfo query(const Circle &other) const = 0;
};
export struct Polygon : public Collider {
  struct vertex {
  private:
    const Polygon &parent;
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
  private:
    const Polygon &parent;
    const vertex &tail, &head;

  public:
    edge(const Polygon &parent, const vertex &tail, const vertex &head)
        : parent{parent}, tail{tail}, head{head} {}

    glm::vec2 localTail() const { return tail.localPos(); }
    glm::vec2 localHead() const { return head.localPos(); }
    glm::vec2 globalTail() const { return localTail() + parent.pos(); }
    glm::vec2 globalHead() const { return localHead() + parent.pos(); }
  };

private:
  const std::vector<vertex> vertices;
  const std::vector<edge> edges;

  friend std::unique_ptr<Polygon>
  std::make_unique<Polygon, const Object &, std::vector<glm::vec2>>(
      const Object &, std::vector<glm::vec2> &&);
  Polygon(const Object &parent, const std::vector<glm::vec2> &points);

public:
  static std::unique_ptr<Polygon> create(const Object &parent,
                                         const unsigned char n,
                                         const double radius,
                                         const double offset);

  const std::vector<vertex> &getVertices() const;
  auto localVertices() const;
  auto globalVertices() const;

  SAT::QueryInfo reverseQuery(const Collider &other) const override;
  SAT::QueryInfo query(const Polygon &other) const override;
  SAT::QueryInfo query(const Circle &other) const override;
};
export struct Circle : public Collider {
  const float radius = 1;

private:
  friend std::unique_ptr<Circle>
  std::make_unique<Circle, const Object &, const float &>(const Object &,
                                                          const float &);
  Circle(const Object &parent, const float radius);

public:
  static std::unique_ptr<Circle> create(const Object &parent,
                                        const float radius);

  SAT::QueryInfo reverseQuery(const Collider &other) const override;
  SAT::QueryInfo query(const Polygon &other) const override;
  SAT::QueryInfo query(const Circle &other) const override;
};
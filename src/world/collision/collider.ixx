export module collider;

import glm;

export struct Object;

import <vector>;
import <optional>;
import <memory>;

// export struct AABB;
import aabb;
// not move constuctible if using incomplete uptr

import poly;
import query_info;

struct Convex;
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
  virtual SAT::QueryInfo query(const Convex &other) const = 0;
  virtual SAT::QueryInfo query(const Circle &other) const = 0;
};
export struct Convex : public Collider {
  struct vertex {
  private:
    const Convex &parent;
    const glm::vec2 pos;

  public:
    vertex(const Convex &parent, const glm::vec2 &pos)
        : parent{parent}, pos{pos} {}

    const glm::vec2 &localPos() const { return pos; }
    glm::vec2 globalPos() const {
      return vertexLocalToGlobal(pos, parent.pos(), parent.rot());
    }
  };
  struct edge {
  private:
    const Convex &parent;
    const vertex &tail, &head;

  public:
    edge(const Convex &parent, const vertex &tail, const vertex &head)
        : parent{parent}, tail{tail}, head{head} {}

    glm::vec2 localTail() const { return tail.localPos(); }
    glm::vec2 localHead() const { return head.localPos(); }
    glm::vec2 globalTail() const { return localTail() + parent.pos(); }
    glm::vec2 globalHead() const { return localHead() + parent.pos(); }
  };

private:
  const std::vector<glm::vec2> vertices;
  std::vector<vertex> vvv;
  std::vector<edge> eee;

  friend std::unique_ptr<Convex>
  std::make_unique<Convex, const Object &, std::vector<glm::vec2>>(
      const Object &, std::vector<glm::vec2> &&);
  Convex(const Object &parent, std::vector<glm::vec2> &&vertices);

public:
  static std::unique_ptr<Convex> create(const Object &parent,
                                        const unsigned char n,
                                        const double radius,
                                        const double offset);

  auto globalVertices() const;

  SAT::QueryInfo reverseQuery(const Collider &other) const override;
  SAT::QueryInfo query(const Convex &other) const override;
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
  SAT::QueryInfo query(const Convex &other) const override;
  SAT::QueryInfo query(const Circle &other) const override;
};
export module collider;

import glm;

export struct Object;

import <vector>;
import <optional>;
import <memory>;

// export struct AABB;
import aabb;
// not move constuctible if using incomplete uptr

import query_info;

struct Convex;
struct Circle;
export struct Collider {
  const Object &parent;

protected:
  std::unique_ptr<AABB> aabb;
  Collider(const Object &parent, std::unique_ptr<AABB> aabb);

public:
  glm::vec2 globalPos() const;
  AABB &getAABB();

  virtual SAT::QueryInfo reverseQuery(const Collider &other) const = 0;
  virtual SAT::QueryInfo query(const Convex &other) const = 0;
  virtual SAT::QueryInfo query(const Circle &other) const = 0;
};
export struct Convex : public Collider {
  struct Edge {
    const glm::vec2 &tail, &head;
  };

private:
  std::vector<glm::vec2> vertices;

  Convex(const Object &parent, std::vector<glm::vec2> &&vertices);

public:
  static std::unique_ptr<Convex> create(const Object &parent,
                                        const unsigned char n,
                                        const double radius,
                                        const double offset);

  SAT::QueryInfo reverseQuery(const Collider &other) const override;
  SAT::QueryInfo query(const Convex &other) const override;
  SAT::QueryInfo query(const Circle &other) const override;
};
export struct Circle : public Collider {
  const double radius = 1;

private:
  Circle(const Object &parent, const double radius);

public:
  static std::unique_ptr<Circle> create(const Object &parent,
                                        const double radius);

  SAT::QueryInfo reverseQuery(const Collider &other) const override;
  SAT::QueryInfo query(const Convex &other) const override;
  SAT::QueryInfo query(const Circle &other) const override;
};
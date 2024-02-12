export module collider;

import glm;

export struct Object;

import vector;
import <memory>;

// export struct AABB;
import aabb;
// not move constuctible if using incomplete uptr

import poly;
import query_info;

export struct Polygon;
export struct Circle;

export struct Collider {
  const Object &parent;

protected:
  std::unique_ptr<AABB> aabb;
  Collider(const Object &parent, std::unique_ptr<AABB> aabb);

public:
  const auto &props() const;
  glm::vec2 pos() const;
  float rot() const;

  AABB &getAABB() { return *aabb; }
  const AABB &getAABB() const { return *aabb; }

  virtual SAT::QueryInfo reverseQuery(const Collider &other) const = 0;
  virtual SAT::QueryInfo query(const Polygon &other) const = 0;
  virtual SAT::QueryInfo query(const Circle &other) const = 0;
};
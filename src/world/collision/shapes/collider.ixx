export module collider;

import glm;

export struct Object;

import vector;
import <memory>;

import aabb;

import poly;
import query_info;

export struct Polygon;
export struct Circle;

export struct Collider {
private:
  aabb_t aabb;
  glm::vec2 position{};
  float rotation = 0;

public:
  Collider(aabb_t &&aabb);

  glm::vec2 pos() const;
  float rot() const;

  aabb_t &getAABB() { return *aabb; }
  const aabb_t &getAABB() const { return *aabb; }

  virtual SAT::QueryInfo reverseQuery(const Collider &other) const = 0;
  virtual SAT::QueryInfo query(const Polygon &other) const = 0;
  virtual SAT::QueryInfo query(const Circle &other) const = 0;
};

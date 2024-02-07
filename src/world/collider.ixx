export module collider;

import glm;

export struct Object;

import <vector>;
import <optional>;
import <memory>;

// export struct AABB;
import aabb;
// not move constuctible if using incomplete uptr

export struct Collider {
  const Object &parent;
  std::unique_ptr<AABB> aabb;

  Collider(const Object &parent, std::unique_ptr<AABB> aabb);

  glm::vec2 globalPos() const;
};
export struct Convex : public Collider {
  std::vector<glm::vec2> vertices;

  Convex(const Object &parent, std::vector<glm::vec2> &&vertices);
  static std::unique_ptr<Convex> create(const Object &parent,
                                        const unsigned char n,
                                        const double radius,
                                        const double offset);
};
export struct Circle : public Collider {
  const double radius = 1;

  Circle(const Object &parent, const double radius);
  static std::unique_ptr<Circle> create(const Object &parent,
                                        const double radius);
};
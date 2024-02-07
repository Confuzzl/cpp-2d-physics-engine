export module object;

import glm;
import <memory>;

import mesh;
import collider;
import app;

import math;

struct ngon_opts {
  const unsigned char sides;
  const double radius = 1;
  const double offset = 0;
  const double mass = 1;
};
struct circle_opts {
  const double radius = 1;
  const double mass = 1;
};
struct obj_opts {
  const glm::vec2 pos{};
  const float rotation = 0;
  const double mass = 1;
  const glm::u8vec3 color = randomColor();
};

export struct Object {
  double mass;

  glm::vec2 pos;
  glm::vec2 velocity{};

  float rotation;
  double angVelocity = 0;

private:
  glm::u8vec3 color;

public:
  glm::uvec3 getColor() const;

  std::unique_ptr<Mesh> mesh;
  std::unique_ptr<Collider> collider;

  Object(ngon_opts &&opts, obj_opts &&obj_opts);

  Object(circle_opts &&opts, obj_opts &&obj_opts);
  Object(std::unique_ptr<Mesh> mesh, std::unique_ptr<Collider> collider,
         obj_opts &&opts);
  ~Object();

  static Object &ngon(ngon_opts &&opts, obj_opts &&obj_opts);
  static Object &circle(circle_opts &&opts, obj_opts &&obj_opts);

private:
  template <typename shape_opts_t>
  static Object &genericCreate(shape_opts_t &&opts, obj_opts &&obj_opts) {
    auto pair = MAIN_SCENE.objects.emplace(
        std::make_unique<Object>(std::move(opts), std::move(obj_opts)));
    auto &it = pair.first;
    return **it;
  }

public:
  void translate(const glm::vec2 &v);
  void rotate(const float r);
};
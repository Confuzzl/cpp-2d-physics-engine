export module ecs_component;

import aabb;
import glm;
import mesh;
import <functional>;
import runtime_array;
import shapes;

export struct BaseFrame;

export namespace ecs {
inline namespace comp {
struct Transformable {
  glm::vec2 position;
  float rotation = 0;
};
struct Physical {
  struct Linear {
    glm::vec2 velocity;
    glm::vec2 acceleration;
    float mass = 1;
  } linear;

  struct Angular {
    float velocity = 0;
    float acceleration = 0;
    float mass = 1;
  } angular;

  float elasticity = 1;

  float frictionStatic = 0;
  float frictionDynamic = 0;

  bool gravity = true;
};

using Boundable = BoundingBox;
using Shapeable = collision::Shape;

using Renderable = Mesh;
struct DirectRenderable {
  std::function<void(BaseFrame *)> draw;
};
} // namespace comp
} // namespace ecs

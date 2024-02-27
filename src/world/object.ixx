module;

#include "util/gl.h"

export module object;

import glm;
import <memory>;

import mesh;
// import collider;
import circle;
import polygon;
import app;

import math;

import color;

export struct base_obj_t {
  struct phys_opts_t {
    float mass = 1;
    glm::vec2 velocity{};
    float angVelocity = 0;
  };

  float mass = 1;
  glm::vec2 velocity{};
  float angVelocity = 0;

  Mesh mesh;
  color_t color = colors::WHITE;

  base_obj_t(const float mass, const glm::vec2 &velocity,
             const float angVelocity, Mesh &&mesh, const color_t &color);

  struct render_opts_t {
    bool showAABB = false;
    GLenum primitive = GL_TRIANGLES;
  };
  virtual void draw(const render_opts_t &opts) const = 0;
};

template <typename colltype> struct object_t;

export template <> struct object_t<Polygon> : base_obj_t, Polygon {
  object_t(const glm::vec2 &pos, const float r, std::vector<glm::vec2> &&points,
           const float mass, const glm::vec2 &velocity, const float angVelocity,
           Mesh &&mesh, const color_t &color);

  static object_t<Polygon> &New(const Polygon::opts_t &poly_opts,
                                const base_obj_t::phys_opts_t &phys_opts = {},
                                const color_t color = colors::WHITE);

  void draw(const render_opts_t &opts) const override;
};
export template <> struct object_t<Circle> : base_obj_t, Circle {
  object_t(const glm::vec2 &pos, const float r, const float radius,
           const float mass, const glm::vec2 &velocity, const float angVelocity,
           Mesh &&mesh, const color_t &color);

  static object_t<Circle> &New(const float radius = 1,
                               const base_obj_t::phys_opts_t &phys_opts = {},
                               const color_t color = colors::WHITE);

  void draw(const render_opts_t &opts) const override;
};

// struct ngon_opts {
//   const unsigned char sides;
//   const float radius = 1;
//   const double offset = 0;
//   const double mass = 1;
// };
//  struct circle_opts {
//    const float radius = 1;
//    const double mass = 1;
//  };
//  struct obj_opts {
//    const glm::vec2 pos{};
//    const float rotation = 0;
//    const double mass = 1;
//    const glm::u8vec3 color = randomColor();
//  };
//
//  export struct Object {
//    struct phys_props {
//
//      glm::vec2 pos{};
//      glm::vec2 velocity{};
//
//      float rotation = 0;
//      float angVelocity = 0;
//
//      double mass = 1;
//    } properties;
//    const phys_props &getProps() const { return properties; }
//
//  private:
//    std::unique_ptr<Mesh> mesh;
//    std::unique_ptr<Collider> collider;
//
//  public:
//    color_t color;
//
//    Mesh &getMesh() { return *mesh; }
//    const Mesh &getMesh() const { return *mesh; }
//    Collider &getCollider() { return *collider; }
//    const Collider &getCollider() const { return *collider; }
//
//    Object(ngon_opts &&opts, obj_opts &&obj_opts);
//    Object(circle_opts &&opts, obj_opts &&obj_opts);
//    Object(std::unique_ptr<Mesh> mesh, std::unique_ptr<Collider> collider,
//           obj_opts &&opts);
//    ~Object();
//
//    static Object &ngon(ngon_opts &&opts, obj_opts &&obj_opts);
//    static Object &circle(circle_opts &&opts, obj_opts &&obj_opts);
//
//  private:
//    template <typename shape_opts_t>
//    static Object &genericCreate(shape_opts_t &&opts, obj_opts &&obj_opts) {
//      auto pair = MAIN_SCENE.objects.emplace(
//          std::make_unique<Object>(std::move(opts), std::move(obj_opts)));
//      auto &it = pair.first;
//      return **it;
//    }
//
//  public:
//    void translate(const glm::vec2 &v);
//    void rotate(const float r);
//  };
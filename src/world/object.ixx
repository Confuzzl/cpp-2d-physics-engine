module;

#include "util/gl.h"

export module object;

import glm;
import <memory>;

import mesh;
import poly;
import circle;
import polygon;
import app;
import math;
import scene;
import color;
import func;
import rendering;
import world_frame;
import camera;
import shader;

export struct base_obj_t {
  struct phys_opts_t {
    float mass = 1;
    glm::vec2 velocity{};
    float angVelocity = 0;
  };

  float mass;
  glm::vec2 velocity;
  float angVelocity;

  color_t color;

  base_obj_t(const float mass, const glm::vec2 &velocity,
             const float angVelocity, const color_t &color);

  struct render_opts_t {
    bool showAABB = false;
    GLenum primitive = GL_TRIANGLES;
  };
  virtual void draw(const render_opts_t &opts = {}) const = 0;
};

export template <typename colltype> struct Object;

export template <> struct Object<Polygon> : base_obj_t, Polygon {
  Mesh mesh;

  Object(const glm::vec2 &pos, const float r, std::vector<glm::vec2> &&points,
         const float mass, const glm::vec2 &velocity, const float angVelocity,
         Mesh &&mesh, const color_t &color)
      : base_obj_t(mass, velocity, angVelocity, color),
        Polygon(pos, r, std::move(points)), mesh{std::move(mesh)} {}

  static Object<Polygon> &New(const Polygon::opts_t &poly_opts,
                              const glm::vec2 pos = {0, 0}, const float r = 0,
                              const color_t color = colors::random(),
                              const base_obj_t::phys_opts_t &phys_opts = {}) {
    std::vector<glm::vec2> vertices =
        ngonVertices(poly_opts.n, poly_opts.r, poly_opts.offset);
    Mesh mesh{
        func::map<glm::vec2, vertex::simple>(vertices, [](const glm::vec2 &v) {
          return vertex::simple{v.x, v.y};
        })};
    auto it =
        MAIN_SCENE.objs
            .emplace(std::make_unique<Object<Polygon>>(
                pos, r, std::move(vertices), phys_opts.mass, phys_opts.velocity,
                phys_opts.angVelocity, std::move(mesh), color))
            .first;
    return static_cast<Object<Polygon> &>(**it);
  }

  void draw(const render_opts_t &opts = {}) const override {
    MAIN_RENDERER.worldFrame.drawMesh(mesh, pos(), rot(), color,
                                      opts.primitive);
    if (opts.showAABB)
      MAIN_RENDERER.worldFrame.drawBoxFromToFixed(aabb.min, aabb.max, 5, color);
  }
};
export template <> struct Object<Circle> : base_obj_t, Circle {
  Object(const glm::vec2 &pos, const float r, const float radius,
         const float mass, const glm::vec2 &velocity, const float angVelocity,
         const color_t &color)
      : base_obj_t(mass, velocity, angVelocity, color), Circle(pos, r, radius) {
  }

  static Object<Circle> &New(const float radius = 1,
                             const glm::vec2 &pos = {0, 0}, const float r = 0,
                             const color_t color = colors::random(),
                             const base_obj_t::phys_opts_t &phys_opts = {}) {
    auto it = MAIN_SCENE.objs
                  .emplace(std::make_unique<Object<Circle>>(
                      pos, r, radius, phys_opts.mass, phys_opts.velocity,
                      phys_opts.angVelocity, color))
                  .first;
    return static_cast<Object<Circle> &>(**it);
  }

  void draw(const render_opts_t &opts = {}) const override {
    MAIN_RENDERER.worldFrame.drawCircle(pos(), radius, color);
    if (opts.showAABB)
      MAIN_RENDERER.worldFrame.drawBoxFromToFixed(aabb.min, aabb.max, 5, color);
  }
};

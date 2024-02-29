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

  float mass = 1;
  glm::vec2 velocity{};
  float angVelocity = 0;

  color_t color = colors::WHITE;

  base_obj_t(const float mass, const glm::vec2 &velocity,
             const float angVelocity, const color_t &color);

  struct render_opts_t {
    bool showAABB = false;
    GLenum primitive = GL_TRIANGLES;
  };
  virtual void draw(const render_opts_t &opts = {}) const = 0;
};

template <typename colltype> struct object_t;

export template <> struct object_t<Polygon> : base_obj_t, Polygon {
  Mesh mesh;

  object_t(const glm::vec2 &pos, const float r, std::vector<glm::vec2> &&points,
           const float mass, const glm::vec2 &velocity, const float angVelocity,
           Mesh &&mesh, const color_t &color)
      : base_obj_t(mass, velocity, angVelocity, color),
        Polygon(pos, r, std::move(points)), mesh{std::move(mesh)} {}

  static object_t<Polygon> &New(const Polygon::opts_t &poly_opts,
                                const glm::vec2 pos = {0, 0}, const float r = 0,
                                const base_obj_t::phys_opts_t &phys_opts = {},
                                const color_t color = colors::WHITE) {
    std::vector<glm::vec2> vertices =
        ngonVertices(poly_opts.n, poly_opts.r, poly_opts.offset);
    Mesh mesh{
        func::map<glm::vec2, vertex::simple>(vertices, [](const glm::vec2 &v) {
          return vertex::simple{v.x, v.y};
        })};
    auto it =
        MAIN_SCENE.objs
            .emplace(std::make_unique<object_t<Polygon>>(
                pos, r, std::move(vertices), phys_opts.mass, phys_opts.velocity,
                phys_opts.angVelocity, std::move(mesh), color))
            .first;
    return static_cast<object_t<Polygon> &>(**it);
  }

  void draw(const render_opts_t &opts = {}) const override {
    MAIN_RENDERER.worldFrame.drawMesh(mesh, pos(), rot(), color);
  }
};
export template <> struct object_t<Circle> : base_obj_t, Circle {
  object_t(const glm::vec2 &pos, const float r, const float radius,
           const float mass, const glm::vec2 &velocity, const float angVelocity,
           const color_t &color)
      : base_obj_t(mass, velocity, angVelocity, color), Circle(pos, r, radius) {
  }

  static object_t<Circle> &New(const float radius = 1,
                               const glm::vec2 &pos = {0, 0}, const float r = 0,
                               const base_obj_t::phys_opts_t &phys_opts = {},
                               const color_t color = colors::WHITE) {
    auto it = MAIN_SCENE.objs
                  .emplace(std::make_unique<object_t<Circle>>(
                      pos, r, radius, phys_opts.mass, phys_opts.velocity,
                      phys_opts.angVelocity, color))
                  .first;
    return static_cast<object_t<Circle> &>(**it);
  }

  void draw(const render_opts_t &opts = {}) const override {
    MAIN_RENDERER.worldFrame.drawCircle(pos(), radius, color);
  }
};
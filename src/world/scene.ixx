export module scene;

import camera;
import <map>;
import <memory>;
// import bvh2;
import glm;
import ecs_manager;

import hash_grid;
import quadtree;

export struct Scene {
  glm::vec2 gravity{0, -1};

  Camera camera{{0, 0}};

  ecs::Manager ecs;

  using Accel = collision::Quadtree;
  Accel data;

  Scene();

  void init();
  void update(const double dt);

private:
  void updatePhysics(const float dt);
  void updateCollisions();
};
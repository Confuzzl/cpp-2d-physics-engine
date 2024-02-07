module scene;

import object;

Scene::Scene() {}
Scene::~Scene() = default;

void Scene::init() {
  Object::ngon({.sides = 3}, {.pos = {0, 0}});
  Object::ngon({.sides = 5}, {.pos = {3, 0}});
}
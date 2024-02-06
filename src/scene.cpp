module scene;

import object;

Scene::Scene() {}
Scene::~Scene() = default;

void Scene::init() {
  Object &obj = Object::ngon({.sides = 5}, {.pos = {0, 0}});
}
module;

#include "util/main_objects.h"

module scene;

import debug;
import ecs_component;

import app;
import frame;
import aabb;
import glm;
import hash_grid;
import shapes;
import <algorithm>;
import <variant>;
import physics;
// import <tuple>;

static auto remove(size_t &ent,
                   const BoundingBox &box = MAIN_SCENE.data.BOUNDS) {
  const bool a = MAIN_SCENE.data.remove(ent, box);
  if (a)
    ECS.deleteEntity(ent);
}

// static template <typename T>
static auto newCircle(ecs::Transformable &&trans, ecs::Physical &&phys,
                      const float radius) {
  BoundingBox box{radius};
  using collision::Circle;
  using ecs::Collidable, ecs::Boundable;

  auto [id, t, p, c, b] =
      MAIN_SCENE.ecs.newEntity(std::move(trans), std::move(phys),
                               Collidable{
                                   .collider{.data{
                                       Circle{.radius = radius},
                                   }},
                               },
                               Boundable{box});
  MAIN_SCENE.data.insert(id, box + t->position);
  return std::make_tuple(id, t, p, std::get_if<Circle>(&c->collider.data), b);
}

Scene::Scene() : data{/*4, 8, 1 << 8*/ 2, 3, 4} {}

void Scene::init() {
  debugln(false, "SCENE INIT");

  using ecs::Transformable, ecs::Physical;

  auto [a, at, ap, ac, ab] = newCircle(
      Transformable{
          .position{-0.1, 0},
          .rotation = 0,
      },
      Physical{
          .linear{
              .velocity{1, 1},
              .mass = 1,
          },
          .angular{},
      },
      0.5);
  auto [b, bt, bp, bc, bb] = newCircle(
      Transformable{
          .position{0, 0},
          .rotation = 0,
      },
      Physical{
          .linear{
              .velocity{-1, 1},
              .mass = 2,
          },
          .angular{},
      },
      0.5);

  // const auto [v1, v2] =
  //     physics::elastic(at->position, ap->linear.velocity, ap->linear.mass,
  //                      bt->position, bp->linear.velocity, bp->linear.mass);
  // println("{} {} {} {}", vec_string(v1), glm::length(v1), vec_string(v2),
  //         glm::length(v2));
  // ap->linear.velocity = v1;
  // bp->linear.velocity = v2;

  ecs.newEntity(ecs::DirectRenderable{.draw = [=](BaseFrame *frame) {
    const auto data = collision::push(*ac, at->position, *bc, bt->position);
    if (data)
      frame->drawArrow({data.a, data.b}, RED);
  }});

  debugln(false, "SCENE COMPLETE\n"
                 "==================================");
}

void Scene::update(const double dt) {
  // debugln(false, "===NEW FRAME===");
  updateCollisions();
  updatePhysics(static_cast<float>(dt));

  data.cleanup();
}

void Scene::updatePhysics(const float dt) {
  using ecs::EntID;
  using namespace ecs::comp;

  for (auto [id, trans, phys] : ecs.viewIDComp<Transformable, Physical>()) {
    auto &[pos, rot] = *trans;
    auto &[lin, ang] = *phys;

    const auto b = ecs.getComponent<Boundable>(id);
    const bool updateTree = b && lin.velocity != glm::vec2{};

    if (updateTree) {
      data.remove(id, b->localBounds + pos);
    }
    {
      auto &[vel, acc, mass] = lin;
      vel += acc * static_cast<float>(dt);
      pos += vel * static_cast<float>(dt);
    }
    if (updateTree) {
      data.insert(id, b->localBounds + pos);
    }
    {
      auto &[vel, acc, mass] = ang;
      vel += acc * static_cast<float>(dt);
      rot += vel * static_cast<float>(dt);
    }
  }
}
void Scene::updateCollisions() {
  using ecs::EntID;
  using namespace ecs::comp;

  std::vector<std::pair<EntID, EntID>> pairs;
  for (const auto [id, trans, bounds] :
       ecs.viewIDComp<Transformable, Boundable>()) {

    const auto box = bounds->localBounds + trans->position;
    for (const auto [other, box] : data.queryAll(box, id)) {
      if (id < other)
        pairs.emplace_back(id, other);
      else
        pairs.emplace_back(other, id);
    }
  }
  std::ranges::sort(pairs, [](const std::pair<EntID, EntID> &a,
                              const std::pair<EntID, EntID> &b) {
    if (a.first == b.first)
      return a.second < b.second;
    return a.first < b.first;
  });
  const auto remove = std::ranges::unique(pairs);
  pairs.erase(remove.begin(), remove.end());

  for (const auto [a, b] : pairs) {
    auto [at, ap, ac] =
        ecs.getComponents<Transformable, Physical, Collidable>(a);
    auto [bt, bp, bc] =
        ecs.getComponents<Transformable, Physical, Collidable>(b);
    using namespace collision;
    ac->collider.visit(
        [=](const Wall &a) {
          bc->collider.visit([=](const Wall &b) {}, [=](const Circle &b) {},
                             [=](const Polygon &b) {});
        },
        [=](const Circle &a) {
          bc->collider.visit([=](const Wall &b) {},
                             [=](const Circle &b) {
                               const auto data = collision::push(
                                   a, at->position, b, bt->position);
                               if (!data)
                                 return;
                               // const auto total = ap->linear.mass +
                               // bp->linear.mass; at->position += data.vec() *
                               // (ap->linear.mass / total); bt->position -=
                               // data.vec() * (bp->linear.mass / total); const
                               // auto [newA, newB] = physics::elastic(
                               //     at->position, ap->linear.velocity,
                               //     ap->linear.mass, bt->position,
                               //     bp->linear.velocity, bp->linear.mass);
                               // ap->linear.velocity = newA;
                               // bp->linear.velocity = newB;
                               ap->linear.velocity = {};
                               bp->linear.velocity = {};
                             },
                             [=](const Polygon &b) {});
        },
        [=](const Polygon &a) {
          bc->collider.visit([=](const Wall &b) {}, [=](const Circle &b) {},
                             [=](const Polygon &b) {});
        });
  }
}
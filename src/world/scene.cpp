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

static auto newWall(ecs::Transformable &&trans, ecs::Physical &&phys,
                    collision::Wall &&wall) {
  using collision::Wall;
  using namespace ecs::comp;

  const auto [v1, v2] =
      wall.getVertices(Transformable{.rotation = trans.rotation});

  phys.linear.mass = INF;
  phys.gravity = false;

  auto [id, t, p, s, b] = MAIN_SCENE.ecs.newEntity(
      std::move(trans), std::move(phys), Shapeable{std::move(wall)},
      Boundable::checked(v1, v2));
  MAIN_SCENE.data.insert(id, *b + t->position);
  return std::make_tuple(id, t, p, &std::get<Wall>(s->data), b);
}
static auto newCircle(ecs::Transformable &&trans, ecs::Physical &&phys,
                      const float radius) {
  using collision::Circle;
  using namespace ecs::comp;

  auto [id, t, p, s, b] = MAIN_SCENE.ecs.newEntity(
      std::move(trans), std::move(phys), Shapeable{Circle{.radius = radius}},
      Boundable{radius});
  MAIN_SCENE.data.insert(id, *b + t->position);
  return std::make_tuple(id, t, p, &std::get<Circle>(s->data), b);
}

Scene::Scene() : data{/*4, 8, 1 << 8*/ 2, 3, 4} {}

void Scene::init() {
  debugln(false, "SCENE INIT");

  using namespace ecs::comp;

  auto [a, at, ap, circle, ab] =
      newCircle({.position{-1, 0.5}},
                {.linear{.velocity{0, 0}, .mass = 0.5},
                 .elasticity = 0.9f,
                 .gravity = true},
                0.2f);

  auto [w, wt, wp, wall, wb] =
      newWall({.position{0, 0}, .rotation = 0},
              {.linear{.mass = 1}, .angular{}, .elasticity = 1},
              {{1, 0}, {-1, 0}, false});

  // const auto [x, y] = collision::push(*wall, *wt, *circle, *at);
  // ecs.newEntity(DirectRenderable{.draw = [=](BaseFrame *frame) {
  //   frame->drawDot(x, 0.02f, RED);
  //   frame->drawDot(y, 0.02f, BLUE);
  // }});

  // auto [b, bt, bp, bc, bb] = newCircle(
  //     Transformable{
  //         .position{1, 0},
  //         .rotation = 0,
  //     },
  //     Physical{
  //         .linear{
  //             .velocity{-1, 0},
  //             .mass = 1,
  //         },
  //         .angular{},
  //     },
  //     0.5);

  // const auto [v1, v2] =
  //     physics::elastic(at->position, ap->linear.velocity,
  //     ap->linear.mass,
  //                      bt->position, bp->linear.velocity,
  //                      bp->linear.mass);
  // println("{} {} {} {}", vec_string(v1), glm::length(v1), vec_string(v2),
  //         glm::length(v2));
  // ap->linear.velocity = v1;
  // bp->linear.velocity = v2;

  // ecs.newEntity(ecs::DirectRenderable{.draw = [=](BaseFrame *frame) {
  //   const auto data = collision::push(*ac, at->position, *bc,
  //   bt->position); if (data)
  //     frame->drawArrow({data.a, data.b}, RED);
  // }});

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
    auto &[lin, ang, e, sf, df, g] = *phys;

    const auto bounds = ecs.getComponent<Boundable>(id);
    const auto shape = ecs.getComponent<Shapeable>(id);
    const bool updateTree =
        bounds && (lin.velocity != glm::vec2{} || ang.velocity != 0);

    if (updateTree) {
      data.remove(id, *bounds + pos);
    }
    {
      auto &[vel, acc, mass] = lin;
      if (g)
        vel += gravity * dt;
      vel += acc * dt;
      pos += vel * dt;
    }
    {
      auto &[vel, acc, mass] = ang;
      vel += acc * dt;
      rot += vel * dt;
    }
    if (updateTree) {
      data.insert(id, *bounds + pos);
    }
  }
}
void Scene::updateCollisions() {
  using ecs::EntID;
  using namespace ecs::comp;

  std::vector<std::pair<EntID, EntID>> pairs;
  for (const auto [id, trans, bounds] :
       ecs.viewIDComp<Transformable, Boundable>()) {
    const auto box = *bounds + trans->position;
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
    auto [at, ap, as] =
        ecs.getComponents<Transformable, Physical, Shapeable>(a);
    auto [bt, bp, bs] =
        ecs.getComponents<Transformable, Physical, Shapeable>(b);
    using namespace collision;
    as->visit(
        [=](const Wall &a) {
          bs->visit(
              [=](const Wall &b) {
                physics::collideWallWall(a, *at, *ap, b, *bt, *bp);
              },
              [=](const Circle &b) {
                physics::collideWallCirc(a, *at, *ap, b, *bt, *bp);
              },
              [=](const Polygon &b) {
                physics::collideWallPoly(a, *at, *ap, b, *bt, *bp);
              });
        },
        [=](const Circle &a) {
          bs->visit(
              [=](const Wall &b) {
                physics::collideWallCirc(b, *bt, *bp, a, *at, *ap);
              },
              [=](const Circle &b) {
                physics::collideCircCirc(a, *at, *ap, b, *bt, *bp);
              },
              [=](const Polygon &b) {
                physics::collideCircPoly(a, *at, *ap, b, *bt, *bp);
              });
        },
        [=](const Polygon &a) {
          bs->visit(
              [=](const Wall &b) {
                physics::collideWallPoly(b, *bt, *bp, a, *at, *ap);
              },
              [=](const Circle &b) {
                physics::collideCircPoly(b, *bt, *bp, a, *at, *ap);
              },
              [=](const Polygon &b) {
                physics::collidePolyPoly(a, *at, *ap, b, *bt, *bp);
              });
        });
  }
}
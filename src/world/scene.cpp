module;

#include "util/gl.h"
#include "util/main_objects.h"

module scene;

import debug;
import ecs_component;

import app;
import frame;
import aabb;
import glm;
// import hash_grid;
import shapes;
import <algorithm>;
// import <variant>;
import physics;
import dispatcher;

static auto remove(size_t &ent,
                   const BoundingBox &box = MAIN_SCENE.data.BOUNDS) {
  const bool a = MAIN_SCENE.data.remove(ent, box);
  if (a)
    ECS.deleteEntity(ent);
}
template <typename T>
static auto newPhysObj(ecs::comp::Physical &&phys, T &&shape) {
  using U = std::remove_cvref_t<T>;
  using collision::Wall;
  using namespace ecs::comp;

  auto [id, p, c] = MAIN_SCENE.ecs.newEntity(
      std::move(phys),
      Collidable{.shape = std::make_unique<U>(std::forward<T>(shape)),
                 .bounds{}});

  c->bounds = c->shape->calcBox(p->position, p->rotation);
  MAIN_SCENE.data.insert(id, c->bounds);
  return std::make_tuple(static_cast<U *>(c->shape.get()), id, p, c);
}
static auto newPhysPoly(ecs::comp::Physical &&phys,
                        collision::Polygon &&shape) {
  using collision::Wall;
  using namespace ecs::comp;

  std::vector<vertex_layout::pos> vertices;
  vertices.reserve(shape.getVertices().size());
  for (const glm::vec2 v : shape.getVertices())
    vertices.emplace_back(v);
  Mesh mesh{std::move(vertices), GL_TRIANGLE_FAN, RED};

  auto [id, p, r, c] = MAIN_SCENE.ecs.newEntity(
      std::move(phys), std::move(mesh),
      Collidable{.shape =
                     std::make_unique<collision::Polygon>(std::move(shape)),
                 .bounds{}});
  c->bounds = c->shape->calcBox(p->position, p->rotation);
  r->color = colors::random_i(id);

  c->bounds = c->shape->calcBox(p->position, p->rotation);
  MAIN_SCENE.data.insert(id, c->bounds);
  return std::make_tuple(static_cast<collision::Polygon *>(c->shape.get()), id,
                         p, c);
}

Scene::Scene() : data{/*4, 8, 1 << 8*/ 2, 3, 4} {}

void Scene::init() {
  debugln(false, "SCENE INIT");

  using namespace ecs::comp;

  newPhysPoly({.position{0.5, 1},
               .linear{.velocity{}, .acceleration{}, .mass = 1},
               .rotation = 0.3,
               .angular{.velocity = 0, .acceleration = 0, .mass = 1},
               .elasticity = 0,
               .frictionStatic = 0,
               .frictionDynamic = 0,
               .gravity = true},
              {{-0.5, -0.5}, {+0.5, -0.5}, {0, 0.5}});
  // newPhysPoly({.position{0, 0},
  //              .linear{.velocity{}, .acceleration{}, .mass = 1},
  //              .rotation = 0,
  //              .angular{.velocity = 0, .acceleration = 0, .mass = 1},
  //              .elasticity = 0,
  //              .frictionStatic = 0,
  //              .frictionDynamic = 0,
  //              .gravity = false},
  //             collision::Polygon::box({1, 1}));

  newPhysObj({.position{0, 0},
              .linear{.velocity{}, .acceleration{}, .mass = INF},
              .rotation = 0,
              .angular{.velocity = 0, .acceleration = 0, .mass = INF},
              .elasticity = 0,
              .frictionStatic = 0,
              .frictionDynamic = 0,
              .gravity = false},
             collision::Wall{{+3, 0}, {-3, 0}, false});

  // static constexpr glm::vec2 corners[4]{{-4, -4}, {+4, -4}, {+4, +4}, {-4,
  // +4}}; static constexpr std::size_t indices[4][2]{{1, 0}, {2, 1}, {3, 2},
  // {0, 3}}; for (const auto [a, b] : indices) {
  //   newPhysObj({.position{0, 0},
  //               .linear{.velocity{}, .acceleration{}, .mass = INF},
  //               .rotation = 0,
  //               .angular{.velocity = 0, .acceleration = 0, .mass = INF},
  //               .elasticity = 0,
  //               .frictionStatic = 0,
  //               .frictionDynamic = 0,
  //               .gravity = false},
  //              collision::Wall{corners[a], corners[b], false});
  // }

  // for (auto i = 0u; i < 10; i++) {
  //   newPhysObj({.position = random_vec({-3, -3}, {3, 3}),
  //               .linear{.velocity = random_vec({-1, 0}, {1, 1}),
  //                       .acceleration{},
  //                       .mass = random_float(0.1f, 10.0f)},
  //               .rotation = 0,
  //               .angular{.velocity = 0, .acceleration = 0, .mass = 1},
  //               .elasticity = 0,
  //               .frictionStatic = 0,
  //               .frictionDynamic = 0,
  //               .gravity = true},
  //              collision::Circle{random_float(0.1f, 0.5f)});
  // }

  // newPhysPoly({.position{0, 0}, .rotation = 0},
  //             {{-0.5, -0.5}, {0.5, -0.5}, {0.2, 0.2}, {-0.3, 0.5}});

  // newPhysObj({.position{0, 1},
  //             .linear{.velocity{}, .acceleration{}, .mass = 1},
  //             .rotation = 0,
  //             .angular{.velocity = 0, .acceleration = 0, .mass = 1},
  //             .elasticity = 1,
  //             .frictionStatic = 0,
  //             .frictionDynamic = 0,
  //             .gravity = true},
  //            collision::Circle{0.5f});
  //  newPhysObj({.position{0, 0},
  //              .linear{.velocity{}, .acceleration{}, .mass = 1},
  //              .rotation = 0,
  //              .angular{.velocity = 0, .acceleration = 0, .mass = 1},
  //              .elasticity = 1,
  //              .frictionStatic = 0,
  //              .frictionDynamic = 0,
  //              .gravity = true},
  //             collision::Circle{0.5f});

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

  for (auto [id, phys] : ecs.viewIDComp<Physical>()) {
    auto &[pos, lin, rot, ang, e, sf, df, g] = *phys;

    const auto collider = ecs.getComponent<Collidable>(id);
    const bool updateTree =
        collider && (lin.velocity != glm::vec2{} || ang.velocity != 0);

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
      const bool r = data.remove(id, collider->bounds);
      collider->bounds = collider->shape->calcBox(pos, rot);
      data.insert(id, collider->bounds);
    }
  }
}
void Scene::updateCollisions() {
  using namespace collision;
  static constexpr SimpleDispatcher<
      ShapeTypes, void, const Shape &,
      std::tuple<glm::vec2 &, glm::vec2 &, const float, float &, float &,
                 const float, const float>>
      handleCollision{[]<typename A, typename B> {
        return &physics::handleCollision<A, B>;
      }};

  using ecs::EntID;
  using namespace ecs::comp;

  std::vector<std::pair<EntID, EntID>> pairs;
  for (const auto [id, p, c] : ecs.viewIDComp<Physical, Collidable>()) {
    const auto box = c->bounds;
    // println(data.elementCount);
    for (const auto [other, box] :
         data.queryAll(box, id /*, &BoundingBox::touches*/)) {
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
    auto [ap, ac] = ecs.getComponents<Physical, Collidable>(a);
    auto [bp, bc] = ecs.getComponents<Physical, Collidable>(b);

    handleCollision(
        *(ac->shape), ap->position, ap->linear.velocity, ap->linear.mass,
        ap->rotation, ap->angular.velocity, ap->angular.mass, ap->elasticity, //
        *(bc->shape), bp->position, bp->linear.velocity, bp->linear.mass,
        bp->rotation, bp->angular.velocity, bp->angular.mass, bp->elasticity);
    ac->bounds = ac->shape->calcBox(ap->position, ap->rotation);
    bc->bounds = bc->shape->calcBox(bp->position, bp->rotation);
  }
}
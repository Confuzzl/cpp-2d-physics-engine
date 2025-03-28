export module physics;

import glm;
import <utility>;
import debug;
import math;
import shapes;
import ecs_component;

export namespace physics {
enum MassCollisionBehavior {
  VELOCITY_TO_ZERO,
  VELOCITY_REVERSE,
};
constexpr auto ZERO_MASS_COLLISION_BEHAVIOR = VELOCITY_TO_ZERO;

// https://en.wikipedia.org/wiki/Elastic_collision#Derivation_of_two_dimensional_solution
std::pair<glm::vec2, glm::vec2> elasticCircles(const glm::vec2 x1,
                                               const glm::vec2 v1, float m1,
                                               const glm::vec2 x2,
                                               const glm::vec2 v2, float m2) {
  if (m1 == m2) {
    if (m1 == INF)
      return {{}, {}};
    if (m1 == 0) {
      if constexpr (ZERO_MASS_COLLISION_BEHAVIOR == VELOCITY_TO_ZERO)
        return {{}, {}};
      return {-v1, -v2};
    }
  }
  if (m1 == INF) {
    m1 = 1;
    m2 = 0;
  } else if (m2 == INF) {
    m1 = 0;
    m2 = 1;
  }

  const float mfrac = 2 / (m1 + m2);
  const auto xdiff = x1 - x2;
  const auto frac2 = glm::dot(v1 - v2, xdiff) / glm::length2(xdiff);
  const auto offset = xdiff * frac2;
  return {v1 - mfrac * m2 * offset, v2 + mfrac * m1 * offset};
}

std::pair<glm::vec2, glm::vec2> elasticGeneric(const glm::vec2 normal,
                                               const glm::vec2 v1, float m1,
                                               const glm::vec2 v2, float m2) {
  if (m1 == m2) {
    if (m1 == INF)
      return {{}, {}};
    if (m1 == 0) {
      if constexpr (ZERO_MASS_COLLISION_BEHAVIOR == VELOCITY_TO_ZERO)
        return {{}, {}};
      return {-v1, -v2};
    }
  }
  if (m1 == INF) {
    m1 = 1;
    m2 = 0;
  } else if (m2 == INF) {
    m1 = 0;
    m2 = 1;
  }

  const float mfrac = 2 / (m1 + m2);
  const auto frac2 = glm::dot(v1 - v2, normal);
  const auto offset = normal * frac2;
  return {v1 - mfrac * m2 * offset, v2 + mfrac * m1 * offset};
}

std::pair<glm::vec2, glm::vec2> elasticGeneric2(const glm::vec2 normal,
                                                const glm::vec2 v1, float m1,
                                                const glm::vec2 v2, float m2) {
  if (m1 == m2) {
    if (m1 == INF)
      return {{}, {}};
    if (m1 == 0) {
      if constexpr (ZERO_MASS_COLLISION_BEHAVIOR == VELOCITY_TO_ZERO)
        return {{}, {}};
      return {-v1, -v2};
    }
  }
  if (m1 == INF) {
    m1 = 1;
    m2 = 0;
  } else if (m2 == INF) {
    m1 = 0;
    m2 = 1;
  }

  glm::vec2 impulse =
      //-2 * glm::dot(v2 - v1, normal) / (1 / m1 + 1 / m2) * normal;
      -2 / (m1 + m2) * glm::dot(v2 - v1, normal) * normal;
  return {{v1 - impulse * m2}, {v2 + impulse * m1}};
}

// https://fotino.me/2d-rigid-body-collision-response/
std::tuple<glm::vec2, float, glm::vec2, float>
resolution(const float restitution, const glm::vec2 normal,    //
           const glm::vec2 v1, const float m1,                 //
           const glm::vec2 r1, const float o1, const float i1, //
           const glm::vec2 v2, const float m2,                 //
           const glm::vec2 r2, const float o2, const float i2) {
  static constexpr auto perpdot = [](const glm::vec2 a, const glm::vec2 b) {
    return a.x * b.y - a.y * b.x;
  };

  const float raxn = perpdot(r1, normal), rbxn = perpdot(r2, normal);
  const float j = -(1 + restitution) *
                  (glm::dot(v1 - v2, normal) + o1 * raxn - o2 * rbxn) /
                  (1 / m1 + 1 / m2 + raxn * raxn / i1 + rbxn * rbxn / i2);
  const glm::vec2 impulse = j * normal;
  const auto va = v1 + impulse / m1, vb = v2 - impulse / m2;
  const auto oa = o1 + perpdot(r1, impulse) / i1,
             ob = o2 - perpdot(r2, impulse) / i2;
  return {va, oa, vb, ob};
}

// std::pair<glm::vec2, glm::vec2>
// resolveImpulse(const float restitution, const glm::vec2 normal,
//                const glm::vec2 v1, const float m1, const glm::vec2 v2,
//                const float m2) {
//   // if (m1 == m2) {
//   //   if (m1 == INF)
//   //     return {{}, {}};
//   //   if (m1 == 0) {
//   //     if constexpr (ZERO_MASS_COLLISION_BEHAVIOR == VELOCITY_TO_ZERO)
//   //       return {{}, {}};
//   //     return {-v1, -v2};
//   //   }
//   // }
//   // if (m1 == INF) {
//   // m1 = 1;
//   // m2 = 0;
//   //} else if (m2 == INF) {
//   // m1 = 0;
//   // m2 = 1;
//   //}
//
//   glm::vec2 impulse =
//       -(1 + restitution) * glm::dot(v2 - v1, normal) / (1 / m1 + 1 / m2) *
//       normal;
//   return {{v1 - impulse / m1}, {v2 + impulse / m2}};
// }

void test() {
  const glm::vec2 x1 = random_vec({-3, -3}, {+3, +3}),
                  x2 = random_vec({-3, -3}, {+3, +3});
  const glm::vec2 v1 = random_vec({-3, -3}, {+3, +3}),
                  v2 = random_vec({-3, -3}, {+3, +3});
  const float m1 = random_float(0.f, 5.f), m2 = random_float(0.f, 5.f);
  for (auto i = 0u; i < 10; i++) {
    const auto [a1, b1] = elasticCircles(x1, v1, m1, x2, v2, m2);
    const auto [a2, b2] =
        elasticGeneric(glm::normalize(x1 - x2), v1, m1, v2, m2);
    const auto [a3, b3] =
        elasticGeneric2(glm::normalize(x1 - x2), v1, m1, v2, m2);
    println("({} {}) ({} {}) ({} {})", vec_string(a1), vec_string(b1),
            vec_string(a2), vec_string(b2), vec_string(a3), vec_string(b3));
  }
}

template <typename A, typename B>
bool snap(const A &a, ecs::Transformable &at, ecs::Physical &ap, const B &b,
          ecs::Transformable &bt, ecs::Physical &bp) {
  const auto data = collision::push(a, at, b, bt);
  if (!data)
    return false;
  const float aSpeed = glm::length(ap.linear.velocity),
              bSpeed = glm::length(bp.linear.velocity);
  const float totalSpeed = aSpeed + bSpeed;
  at.position += data.vec() * aSpeed / totalSpeed;
  bt.position -= data.vec() * bSpeed / totalSpeed;
  return true;
}

glm::vec2 elasticClamp(const glm::vec2 v, const float e) {
  static constexpr auto EPSILON = 1.0E-4f;
  const auto out = v * e;
  if (glm::length2(out) < EPSILON)
    return {};
  return out;
}

void collideWallWall(const collision::Wall &a, ecs::Transformable &at,
                     ecs::Physical &ap, const collision::Wall &b,
                     ecs::Transformable &bt, ecs::Physical &bp) {}
void collideWallCirc(const collision::Wall &a, ecs::Transformable &at,
                     ecs::Physical &ap, const collision::Circle &b,
                     ecs::Transformable &bt, ecs::Physical &bp) {
  if (!snap(a, at, ap, b, bt, bp))
    return;

  const auto [newA, newB] =
      elasticGeneric2(a.normal(at), ap.linear.velocity, ap.linear.mass,
                      bp.linear.velocity, bp.linear.mass);
  ap.linear.velocity = elasticClamp(newA, ap.elasticity);
  bp.linear.velocity = elasticClamp(newB, bp.elasticity);
}
void collideWallPoly(const collision::Wall &a, ecs::Transformable &at,
                     ecs::Physical &ap, const collision::Polygon &b,
                     ecs::Transformable &bt, ecs::Physical &bp) {}

void collideCircCirc(const collision::Circle &a, ecs::Transformable &at,
                     ecs::Physical &ap, const collision::Circle &b,
                     ecs::Transformable &bt, ecs::Physical &bp) {
  if (!snap(a, at, ap, b, bt, bp))
    return;

  const auto [newA, newB] =
      elasticCircles(at.position, ap.linear.velocity, ap.linear.mass,
                     bt.position, bp.linear.velocity, bp.linear.mass);
  ap.linear.velocity = elasticClamp(newA, ap.elasticity);
  bp.linear.velocity = elasticClamp(newB, bp.elasticity);
}
void collideCircPoly(const collision::Circle &a, ecs::Transformable &at,
                     ecs::Physical &ap, const collision::Polygon &b,
                     ecs::Transformable &bt, ecs::Physical &bp) {}

void collidePolyPoly(const collision::Polygon &a, ecs::Transformable &at,
                     ecs::Physical &ap, const collision::Polygon &b,
                     ecs::Transformable &bt, ecs::Physical &bp) {}
} // namespace physics
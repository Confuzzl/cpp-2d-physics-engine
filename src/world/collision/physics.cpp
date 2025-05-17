module physics;

import shapes;

using namespace collision;

namespace physics {
template <ExtremeCollisionBehavior b>
static Resolution extremeMassCollision(const glm::vec2 v1, const glm::vec2 v2) {
  if constexpr (b == VELOCITY_TO_ZERO)
    return {{}, {}};
  return {-v1, -v2};
}

static std::pair<bool, Resolution> massCheck(const glm::vec2 v1, float &m1,
                                             const glm::vec2 v2, float &m2) {
  if (m1 == m2) {
    if (m1 == INF)
      return {true,
              extremeMassCollision<INFINITE_MASS_COLLISION_BEHAVIOR>(v1, v2)};
    if (m1 == 0)
      return {true, extremeMassCollision<ZERO_MASS_COLLISION_BEHAVIOR>(v1, v2)};
  }
  if (m1 == INF) {
    m1 = 1;
    m2 = 0;
  } else if (m2 == INF) {
    m1 = 0;
    m2 = 1;
  }
  return {false, {}};
}

// https://en.wikipedia.org/wiki/Elastic_collision#Derivation_of_two_dimensional_solution
Resolution elasticCircles(const glm::vec2 x1, const glm::vec2 v1, float m1,
                          const glm::vec2 x2, const glm::vec2 v2, float m2) {
  if (const auto [r, v] = massCheck(v1, m1, v2, m2); r)
    return v;

  const auto xdiff = x1 - x2;
  const auto frac2 = glm::dot(v1 - v2, xdiff) / glm::length2(xdiff);
  const auto impulse = 2 / (m1 + m2) * xdiff * frac2;
  return {v1 - impulse * m2, v2 + impulse * m1};
}
Resolution elasticGeneric(const glm::vec2 normal, const glm::vec2 v1, float m1,
                          const glm::vec2 v2, float m2) {
  if (const auto [r, v] = massCheck(v1, m1, v2, m2); r)
    return v;

  const glm::vec2 impulse =
      //-2 * glm::dot(v2 - v1, normal) / (1 / m1 + 1 / m2) * normal;
      //....................normal can be either direction
      -2 / (m1 + m2) * glm::dot(v2 - v1, normal) * normal;
  return {v1 - impulse * m2, v2 + impulse * m1};
}
// https://fotino.me/2d-rigid-body-collision-response/
FullResolution elasticGenericFull(const float restitution,
                                  const glm::vec2 normal,             //
                                                                      //
                                  const glm::vec2 r1,                 //
                                  const glm::vec2 v1, const float m1, //
                                  const float o1, const float i1,     //
                                                                      //
                                  const glm::vec2 r2,                 //
                                  const glm::vec2 v2, const float m2, //
                                  const float o2, const float i2) {
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

glm::vec2 elasticClamp(const glm::vec2 v, const float e) {
  static constexpr auto EPSILON = 1.0E-4f;
  const auto out = v * e;
  if (glm::length2(out) < EPSILON)
    return {};
  return out;
}

template <>
Resolution collideImpl(const glm::vec2 normal, const Circle &a,
                       const glm::vec2 va, const float ma, const Circle &b,
                       const glm::vec2 vb, const float mb) {
  return elasticCircles(a.center, va, ma, b.center, vb, mb);
}

void test() {
  glm::vec2 normal{1, 0};
  println(vec_string(glm::dot(glm::vec2{2, 3}, normal) * normal));
  normal *= -1;
  println(vec_string(glm::dot(glm::vec2{2, 3}, normal) * normal));

  // const collision::Circle a{1};
  // const collision::Circle b{1};

  // const auto push = collision::push(a, {}, 0, b, {}, 0);
  // println(vec_string(push.vec()));

  // const glm::vec2 x1 = random_vec({-3, -3}, {+3, +3}),
  //                 x2 = random_vec({-3, -3}, {+3, +3});
  // const glm::vec2 v1 = random_vec({-3, -3}, {+3, +3}),
  //                 v2 = random_vec({-3, -3}, {+3, +3});
  // const float m1 = random_float(0.f, 5.f), m2 = random_float(0.f, 5.f);
  // for (auto i = 0u; i < 10; i++) {
  //   const auto [a1, b1] = elasticCircles(x1, v1, m1, x2, v2, m2);
  //   const auto [a2, b2] =
  //       elasticGeneric(glm::normalize(x1 - x2), v1, m1, v2, m2);
  //   const auto [a3, b3] =
  //       elasticGeneric2(glm::normalize(x1 - x2), v1, m1, v2, m2);
  //   println("({} {}) ({} {}) ({} {})", vec_string(a1), vec_string(b1),
  //           vec_string(a2), vec_string(b2), vec_string(a3), vec_string(b3));
  // }
}
} // namespace physics
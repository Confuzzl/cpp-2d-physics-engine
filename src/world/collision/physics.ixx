export module physics;

import glm;
import <utility>;
import debug;
import math;
import shapes;
import ecs_component;

namespace physics {
using namespace collision;

using Resolution = std::tuple<glm::vec2, glm::vec2>;
using FullResolution = std::tuple<glm::vec2, float, glm::vec2, float>;

enum ExtremeCollisionBehavior {
  VELOCITY_TO_ZERO,
  VELOCITY_REVERSE,
};
constexpr auto ZERO_MASS_COLLISION_BEHAVIOR = VELOCITY_TO_ZERO;
constexpr auto INFINITE_MASS_COLLISION_BEHAVIOR = VELOCITY_TO_ZERO;

Resolution elasticCircles(const glm::vec2 x1, const glm::vec2 v1, float m1,
                          const glm::vec2 x2, const glm::vec2 v2, float m2);
Resolution elasticGeneric(const glm::vec2 normal, const glm::vec2 v1, float m1,
                          const glm::vec2 v2, float m2);
FullResolution elasticGenericFull(const float restitution,
                                  const glm::vec2 normal,             //
                                                                      //
                                  const glm::vec2 r1,                 //
                                  const glm::vec2 v1, const float m1, //
                                  const float o1, const float i1,     //
                                                                      //
                                  const glm::vec2 r2,                 //
                                  const glm::vec2 v2, const float m2, //
                                  const float o2, const float i2);

template <typename A, typename B>
std::tuple<CollisionData, glm::vec2, glm::vec2>
popOut(const A &a, const glm::vec2 va, float ma, //
       const B &b, const glm::vec2 vb, float mb) {
  const CollisionData data = collision::queryCollision(a, b);
  if (!data)
    return {data, {}, {}};
  const float aSpeed = glm::length(va), bSpeed = glm::length(vb);
  const float totalSpeed = aSpeed + bSpeed;

  // depends on mass now
  if (aSpeed == 0 && bSpeed == 0) {
    if (ma == mb)
      return {data, data.vec() * 0.5f, -data.vec() * 0.5f};
    if (ma == INF)
      return {data, {}, -data.vec()};
    if (mb == INF)
      return {data, data.vec(), {}};

    const float totalMass = ma + mb;
    return {data, data.vec() * ma / totalMass, -data.vec() * mb / totalMass};
  }
  return {data, data.vec() * aSpeed / totalSpeed,
          -data.vec() * bSpeed / totalSpeed};
}

glm::vec2 elasticClamp(const glm::vec2 v, const float e);

template <typename A, typename B>
Resolution collideImpl(const glm::vec2 normal, const A &a, const glm::vec2 va,
                       const float ma, const B &b, const glm::vec2 vb,
                       const float mb) {
  return elasticGeneric(normal, va, ma, vb, mb);
}
template <>
Resolution collideImpl(const glm::vec2 normal, const Circle &a,
                       const glm::vec2 va, const float ma, const Circle &b,
                       const glm::vec2 vb, const float mb);
} // namespace physics

export namespace physics {
template <typename A, typename B>
void handleCollision(const A &a,                                   //
                     glm::vec2 &pa, glm::vec2 &va, const float ma, //
                     float &ra, float &oa, const float ia,         //
                     const float ea,                               //
                     const B &b,                                   //
                     glm::vec2 &pb, glm::vec2 &vb, const float mb, //
                     float &rb, float &ob, const float ib,         //
                     const float eb) {
  const A prePopA = a.transform(pa, ra);
  const B prePopB = b.transform(pb, rb);
  const auto [data, pushA, pushB] =
      popOut<A, B>(prePopA, va, ma, prePopB, vb, mb);
  // println("push {} {}", vec_string(pushA), vec_string(pushB));
  if (!data)
    return;
  pa += pushA;
  pb += pushB;

  // avoid rotation overhead
  const A postPopA = prePopA.translate(pushA);
  const B postPopB = prePopB.translate(pushB);

  const auto [va2, vb2] =
      collideImpl<A, B>(data.normal(), postPopA, va, ma, postPopB, vb, mb);
  va = elasticClamp(va2, ea);
  vb = elasticClamp(vb2, eb);
  // println("vel {} {}", vec_string(va), vec_string(vb));
}

void test();
} // namespace physics
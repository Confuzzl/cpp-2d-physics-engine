export module physics;

import glm;
import <utility>;
import debug;
import math;

export namespace physics {
constexpr float IMMOVABLE = INF;

enum ZeroMassCollision {
  VELOCITY_TO_ZERO,
  VELOCITY_REVERSE,
};
constexpr ZeroMassCollision ZERO_MASS_COLLISION_BEHAVIOR = VELOCITY_TO_ZERO;

// https://en.wikipedia.org/wiki/Elastic_collision#Derivation_of_two_dimensional_solution
std::pair<glm::vec2, glm::vec2>
elasticCircles(const glm::vec2 x1, const glm::vec2 v1, const float m1,
               const glm::vec2 x2, const glm::vec2 v2, const float m2) {
  if (m1 == 0 && m2 == 0) {
    if constexpr (ZERO_MASS_COLLISION_BEHAVIOR == VELOCITY_TO_ZERO)
      return {{}, {}};
    else {
      const auto xdiff = x1 - x2;
      const auto frac2 = glm::dot(v1 - v2, xdiff) / glm::length2(xdiff);
      const auto offset = xdiff * frac2;
      return {x1 - offset, v2 + offset};
    }
  }

  const float mfrac = 2 / (m1 + m2);
  const auto xdiff = x1 - x2;
  const auto frac2 = glm::dot(v1 - v2, xdiff) / glm::length2(xdiff);
  const auto offset = xdiff * frac2;
  return {v1 - mfrac * m2 * offset, v2 + mfrac * m1 * offset};
}
} // namespace physics
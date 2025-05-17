export module sat;

import math;
import glm;
import <vector>;
import <algorithm>;

import debug;

export namespace collision {
struct Polygon;

namespace SAT {
struct Axis {
  float minA = +INF, maxA = -INF;
  float minB = +INF, maxB = -INF;
  glm::vec2 direction;

  Axis(const glm::vec2 direction) : direction{glm::normalize(direction)} {}
  Axis(const float depth) : minA{0}, maxA{depth}, minB{0}, maxB{depth} {}

private:
  void project(float &min, float &max, const glm::vec2 p) {
    const float dot = glm::dot(direction, p);
    min = std::min(min, dot);
    max = std::max(max, dot);
  }

public:
  void projectA(const glm::vec2 p) { project(minA, maxA, p); }
  void projectB(const glm::vec2 p) { project(minB, maxB, p); }

  float depth() const {
    static constexpr auto contains = [](const float minA, const float maxA,
                                        const float minB, const float maxB) {
      return (minA < minB && minB < maxA) && (minA < maxB && maxB < maxA);
    };

    auto d = std::min(maxA, maxB) - std::max(minA, minB);
    //  https://dyn4j.org/2010/01/sat/#sat-contain
    if (contains(minA, maxA, minB, maxB) || contains(minB, maxB, minA, maxA)) {
      const auto mins = std::abs(minA - minB);
      const auto maxs = std::abs(maxA - maxB);
      d += std::min(mins, maxs);
    }
    return d;
  }
  bool intersecting() const { return depth() > 0; }
};

struct DepthInfo {
  const Polygon *poly = nullptr;
  glm::vec2 normal;
  Axis axis;

  float depth() const { return axis.depth(); }
};

// enum PROJECTION_STATE : bool { NONE, INTERSECTION };
} // namespace SAT
} // namespace collision
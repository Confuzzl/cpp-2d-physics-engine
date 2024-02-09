export module query_info;

import glm;
import math;

export namespace SAT {
struct min_depth {
  static constexpr float INIT_VALUE = F_INF_POS;
  static constexpr bool compare(const float current, const float old) {
    return current < old;
  }
};
struct max_depth {
  static constexpr float INIT_VALUE = F_INF_NEG;
  static constexpr bool compare(const float current, const float old) {
    return current > old;
  }
};
using depth_func = min_depth;

struct QueryInfo {
  bool collision = false;
  glm::vec2 pointA{};
  glm::vec2 pointB{};
  glm::vec2 normalA{};
  float depth = depth_func::INIT_VALUE;

  void calculateNormal() { normalA = glm::normalize(pointA - pointB); }
  glm::vec2 getPush() {
    if (!collision)
      return {};
    return normalA * depth;
  }

  void setPoints(const glm::vec2 &a, const glm::vec2 &b, const bool reverse) {
    if (reverse) {
      pointA = b;
      pointB = a;
    } else {
      pointA = a;
      pointB = b;
    }
  }

  static QueryInfo NONE() { return {}; }
};
} // namespace SAT
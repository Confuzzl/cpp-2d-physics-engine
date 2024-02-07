export module query_info;

import glm;
import <optional>;

export namespace SAT {
struct QueryInfo {
  bool collision = false;
  glm::vec2 pointA;
  glm::vec2 pointB;
  std::optional<glm::vec2> normalA = std::nullopt;
  float depth;

  void calculateNormal() { normalA = glm::normalize(pointA - pointB); }
  glm::vec2 getPush() {
    if (!collision || !normalA)
      return {};
    return normalA.value() * depth;
  }

  // static QueryInfo
  static QueryInfo NONE() { return {}; }
};
} // namespace SAT
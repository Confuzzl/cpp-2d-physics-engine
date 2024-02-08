export module poly;

import <vector>;
import <numbers>;
import <cmath>;

import glm;

export std::vector<glm::vec2> ngonVertices(const unsigned char n,
                                           const double radius,
                                           const double offset = 0) {
  std::vector<glm::vec2> out{};
  out.reserve(n);
  const double factor = 2 * std::numbers::pi / n;
  for (unsigned char i = 0; i < n; i++) {
    const double theta = factor * i + offset;
    out.emplace_back(static_cast<float>(radius * std::cos(theta)),
                     static_cast<float>(radius * std::sin(theta)));
  }
  return out;
}

export glm::vec2 vertexLocalToGlobal(const glm::vec2 &v, const glm::vec2 &t,
                                     const float rot) {
  const float sin = std::sinf(rot), cos = std::cosf(rot);
  return glm::vec2{v.x * cos - v.y * sin, v.x * cos + v.y * sin} + t;
}
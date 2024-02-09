module axis;

Axis::Axis(const glm::vec2 &direction) : direction{direction} {}

void Axis::project(float &min, float &max, const glm::vec2 &p) {
  const float dot = glm::dot(direction, p);
  min = std::fminf(min, dot);
  max = std::fmaxf(max, dot);
}
void Axis::projectToA(const glm::vec2 &p) { project(minA, maxA, p); }
void Axis::projectToB(const glm::vec2 &p) { project(minB, maxB, p); }

float Axis::depth() const {
  return std::fminf(minA, minB) - std::fmaxf(maxA, maxB);
}
bool Axis::isIntersecting() const { return depth() < 0; }

import polygon;
void Axis::projectPolygons(const Polygon &a, const Polygon &b) {
  for (const glm::vec2 &v : a.localVertices())
    projectToA(v);
  for (const glm::vec2 &v : b.localVertices())
    projectToB(v);
}
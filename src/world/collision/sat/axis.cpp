module axis;

import debug;

axis_t::axis_t(const glm::vec2 &direction) : direction{glm::normalize(direction)} {}

void axis_t::project(float &min, float &max, const glm::vec2 &p) {
  const float dot = glm::dot(direction, p);
  min = std::fminf(min, dot);
  max = std::fmaxf(max, dot);
}
void axis_t::projectToA(const glm::vec2 &p) { project(minA, maxA, p); }
void axis_t::projectToB(const glm::vec2 &p) { project(minB, maxB, p); }

float axis_t::depth() const {
  return std::fmaxf(minA, minB) - std::fminf(maxA, maxB);
}
bool axis_t::isIntersecting() const { return depth() < 0; }

import polygon;
void axis_t::projectPolygons(const Polygon &a, const Polygon &b) {
  for (const glm::vec2 &v : a.globalVertices())
    projectToA(v);
  for (const glm::vec2 &v : b.globalVertices())
    projectToB(v);
}
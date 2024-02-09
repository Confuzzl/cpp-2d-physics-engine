export module axis;

import glm;
import math;

export struct Polygon;

export struct Axis {
  float minA = F_INF_POS, maxA = F_INF_NEG;
  float minB = F_INF_POS, maxB = F_INF_NEG;

  const glm::vec2 direction;

  Axis(const glm::vec2 &direction);

  void project(float &min, float &max, const glm::vec2 &p);
  void projectToA(const glm::vec2 &p);
  void projectToB(const glm::vec2 &p);

  float depth() const;
  bool isIntersecting() const;

  void projectPolygons(const Polygon &a, const Polygon &b);
};
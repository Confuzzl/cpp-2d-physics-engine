export module circcirc;

import circle;
import query_info;
import glm;

export namespace SAT {
QueryInfo queryCircCirc(const Circle &a, const Circle &b) {
  const float distance2 = glm::distance2(a.pos(), b.pos());
  const float minDistance2 = (a.radius + b.radius) * (a.radius + b.radius);
  if (distance2 > minDistance2)
    return QueryInfo::NONE();
  QueryInfo out{};

  return out;
}
} // namespace SAT
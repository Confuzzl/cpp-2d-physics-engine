export module polypoly;

import collider;
import query_info;

export namespace SAT {
QueryInfo queryPolyPoly(const Convex &a, const Convex &b) {
  return QueryInfo::NONE();
}
} // namespace SAT
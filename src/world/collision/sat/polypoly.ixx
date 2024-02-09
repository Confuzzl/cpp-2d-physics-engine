export module polypoly;

import collider;
import query_info;

export namespace SAT {
QueryInfo queryPolyPoly(const Polygon &a, const Polygon &b) {
  return QueryInfo::NONE();
}
} // namespace SAT
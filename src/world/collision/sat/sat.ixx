export module sat;

import query_info;
import collider;
import debug;

export namespace SAT {
template <bool aabb_check = true>
QueryInfo queryCollision(const Collider &a, const Collider &b) {
  if constexpr (aabb_check) {
    if (!a.getAABB().intersects(b.getAABB()))
      return QueryInfo::NONE();
  }
  return b.reverseQuery(a);
}
} // namespace SAT
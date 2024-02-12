export module sat;

import query_info;
import collider;
import debug;

export namespace SAT {
QueryInfo queryCollision(const Collider &a, const Collider &b) {
  if (!a.getAABB().intersects(b.getAABB()))
    return QueryInfo::NONE();
  return b.reverseQuery(a);
}
} // namespace SAT
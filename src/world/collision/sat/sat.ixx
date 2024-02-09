export module sat;

import query_info;
import collider;
import debug;

export namespace SAT {
QueryInfo queryCollision(const Collider &a, const Collider &b) {
  return b.reverseQuery(a);
}
} // namespace SAT
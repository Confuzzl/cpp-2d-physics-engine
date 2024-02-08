export module sat;

import <typeinfo>;
import query_info;
import collider;
import <stdexcept>;
import debug;

import polypoly;
import circcirc;
import polycirc;

export namespace SAT {
QueryInfo queryCollision(const Collider &a, const Collider &b) {
  return b.reverseQuery(a);
}

bool minDepth(const float value, const float old) { return value > old; }
bool maxDepth(const float value, const float old) { return value < old; }

const auto &foo = minDepth;

} // namespace SAT
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
// probably more performant way w double dispatch
QueryInfo queryCollision(const Collider &a, const Collider &b) {
  return b.reverseQuery(a);
  // static const std::type_info &CONVEX_TYPE = typeid(Convex),
  //                             &CIRCLE_TYPE = typeid(Circle);

  // const std::type_info &typeA = typeid(a), &typeB = typeid(b);
  // if (typeA == CONVEX_TYPE) {
  //   if (typeB == CONVEX_TYPE)
  //     return queryPolyPoly(reinterpret_cast<const Convex &>(a),
  //                          reinterpret_cast<const Convex &>(b));
  //   if (typeB == CIRCLE_TYPE)
  //     return queryPolyCirc(reinterpret_cast<const Convex &>(a),
  //                          reinterpret_cast<const Circle &>(b), false);
  // } else if (typeA == CIRCLE_TYPE) {
  //   if (typeB == CONVEX_TYPE)
  //     return queryPolyCirc(reinterpret_cast<const Convex &>(b),
  //                          reinterpret_cast<const Circle &>(a), true);
  //   if (typeB == CIRCLE_TYPE)
  //     return queryCircCirc(reinterpret_cast<const Circle &>(a),
  //                          reinterpret_cast<const Circle &>(b));
  // }
  // throw std::runtime_error{std::format("Incompatable types: A: {} B: {}",
  //                                      typeA.name(), typeB.name())};
}
} // namespace SAT
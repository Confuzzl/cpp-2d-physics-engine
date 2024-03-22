module;

// #include <span>
#include <algorithm>
#include <ranges>

export module bvh;

import vector;
import aabb;
import glm;

// import <ranges>;

export namespace spatial {
namespace bvh {
using index_t = int;

template <typename vec_t = glm::vec2> struct tree_t {
  using aabb_t = gen_aabb_t<vec_t>;
  static constexpr unsigned int MIN_OBJECTS_PER_LEAF = 2;

  struct node_t {
    index_t parent = -1;
    index_t left = -1, right = -1;
    aabb_t bounds{};

    struct children_t {
      unsigned int start = 0;
      unsigned int end = 0;
    } objects{};

    node_t() {}

    bool isLeaf() const { return objects.start == objects.end; }
  };

  enum class CONSTRUCT_METHOD { TOP_DOWN, BOTTOM_UP };
  enum class SPLIT_METHOD { OBJ_MED, OBJ_MEAN, SPACE_MID };

  std::vector<aabb_t *> objects;
  std::vector<node_t> nodes{};

  // struct axes_t {
  //   struct axis_t {
  //     struct data_t {
  //       float val;
  //       unsigned int objIndex;
  //     };
  //     std::vector<data_t> points{};
  //   };
  //   axis_t x{}, y{};
  //   axes_t(const std::vector<aabb_t *> &objs) {
  //     x.points.reserve(objs.size());
  //     y.points.reserve(objs.size());
  //     for (unsigned int i = 0; i < objs.size(); i++) {
  //       const vec_t median = objs[i]->median();
  //       x.points.emplace_back(axis_t::data_t{median.x, i});
  //       y.points.emplace_back(axis_t::data_t{median.y, i});
  //     }
  //   }
  // } axes;

  tree_t(std::vector<aabb_t *> &&objs) : objects{std::move(objs)} {
    nodes.resize(objects.size() * 2 - 1);
    for (const aabb_t *box : objects) {
    }
  }

  aabb_t computeBounds(const unsigned int start, const unsigned int end) {
    aabb_t out{};
    for (const aabb_t *obj :
         std::ranges::subrange{objects.begin() + start, objects.begin() + end})
      out.expand(*obj);
    return out;
  }

  void topDown() { topDownRecurse(0, 0, nodes.size()); }
  void topDownRecurse(const unsigned int workingNodeIndex,
                      const unsigned int start, const unsigned int end) {
    node_t &working_node = nodes[workingNodeIndex];
    working_node.bounds = computeBounds(start, end);
    const unsigned int objCount = end - start;
    if (objCount <= MIN_OBJECTS_PER_LEAF) {
      working_node.objects = {.start = start, .end = end};
    } else {
      working_node.left = workingNodeIndex * 2;
      working_node.right = working_node.left + 1;

      const vec_t size = working_node.bounds.size();
      // longest axis (x > y -> 0)
      const bool axis = size.x < size.y;
      const vec_t::value_type midpoint = size[axis] * 0.5;
      const auto part =
          std::partition(objects.begin() + start, objects.begin() + end,
                         [midpoint, axis](const aabb_t *obj) {
                           return obj->median()[axis] < midpoint;
                         });
      const unsigned int partIndex = std::distance(objects.begin(), part);
    }
  }
};
} // namespace bvh
} // namespace spatial
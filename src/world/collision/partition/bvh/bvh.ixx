module;

// #include <span>
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

  index_t root;
  std::vector<aabb_t *> objects;
  std::vector<node_t> nodes{};

  struct {
  } axes;

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

  void topDown() { topDownRecurse(nodes[0], 0, nodes.size()); }
  void topDownRecurse(node_t &working_node, const unsigned int start,
                      const unsigned int end) {
    working_node.bounds = computeBounds(start, end);
    const unsigned int objCount = end - start;
    if (objCount <= MIN_OBJECTS_PER_LEAF) {
      working_node.objects = {.start = start, .end = end};
    } else {
    }
  }
};
} // namespace bvh
} // namespace spatial
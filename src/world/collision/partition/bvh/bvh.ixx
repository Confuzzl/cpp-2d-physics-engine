export module bvh;

import vector;
import aabb;

import <ranges>;

export namespace spatial {
namespace bvh {
using index_t = int;

template <typename vec_t = glm::vec2> struct tree_t {
  using aabb_t = gen_aabb_t<vec_t>;

  struct node_t {
    index_t parent = -1;
    index_t child1 = -1, child2 = -1;
    aabb_t bounds{};

    std::vector<aabb_t *> objects{};

    bool isLeaf() const { return objects.size() != 0; }
  };

  enum class CONSTRUCT_METHOD { TOP_DOWN, BOTTOM_UP };
  enum class SPLIT_METHOD { OBJ_MED, OBJ_MEAN, SPACE_MID };

  index_t root;
  std::vector<aabb_t> boxes;
  std::vector<node_t> nodes{};

  tree_t(const std::vector<aabb_t> &boxes) : boxes{boxes} {
    for (const aabb_t &box : boxes) {
    }
  }

  aabb_t computeBounds(const unsigned int start, const unsigned int end) {
    aabb_t out{};
    for (unsigned int i = start; i < end; i++) {
      out.expand(boxes[i]);
    }
    return out;
  }

  static constexpr unsigned MIN_OBJECTS_PER_LEAF = 1;
  void topDown(node_t &working_node, const unsigned int start,
               const unsigned int end) {
    const auto size = end - start;
    working_node.bounds = computeBounds(start, end);
    if (size <= MIN_OBJECTS_PER_LEAF) {
      working_node.objects.emplace_back();
    } else {
    }
  }
};
} // namespace bvh
} // namespace spatial
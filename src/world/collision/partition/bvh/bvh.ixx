module;

#include <span>

export module bvh;

import vector;
import aabb;
import glm;

import <ranges>;

export namespace spatial {
namespace bvh {
using index_t = int;

template <typename vec_t = glm::vec2> struct tree_t {
  using aabb_t = gen_aabb_t<vec_t>;
  static constexpr unsigned int MIN_OBJECTS_PER_LEAF = 1;

  struct node_t {
    index_t parent = -1;
    index_t child1 = -1, child2 = -1;
    aabb_t bounds{};

    struct children_t {
      aabb_t *start = nullptr;
      unsigned int size = 0;
    } _objects{};
    auto objects() { return std::span{_objects.start, _objects.size}; }

    node_t() {}

    bool isLeaf() const { return _objects.size != 0; }
  };

  enum class CONSTRUCT_METHOD { TOP_DOWN, BOTTOM_UP };
  enum class SPLIT_METHOD { OBJ_MED, OBJ_MEAN, SPACE_MID };

  index_t root;
  std::vector<aabb_t *> boxes;
  std::vector<node_t *> nodes{};

  struct {
  } axes;

  tree_t(const std::vector<aabb_t *> &boxes) : boxes{boxes} {
    nodes.reserve(boxes.size() * 2 - 1);
    for (const aabb_t *box : boxes) {
    }
  }

  aabb_t computeBounds(const unsigned int start, const unsigned int end) {
    aabb_t out{};
    for (unsigned int i = start; i < end; i++) {
      out.expand(*boxes[i]);
    }
    return out;
  }

  void topDown(node_t *working_node, const unsigned int start,
               const unsigned int end) {
    const auto size = end - start;
    working_node->bounds = computeBounds(start, end);
    if (size <= MIN_OBJECTS_PER_LEAF) {
      working_node->_objects = {.start = boxes[start], .size = size};
    } else {
      const unsigned int part = 1;
      topDown(nodes[working_node->child1], 0, part);
      topDown(nodes[working_node->child2], part, size);
    }
  }
};
} // namespace bvh
} // namespace spatial
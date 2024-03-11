export module bvh;

import vector;
import aabb;

export namespace spatial {
namespace bvh {
using index_t = int;

struct node_t {
  index_t parent = -1;
  index_t child1 = -1, child2 = -1;
  const aabb_t *object = nullptr;

  bool isLeaf() const { return object != nullptr; }
};
struct tree_t {
  index_t root;
  std::vector<node_t> nodes{};

  tree_t(const std::vector<aabb_t> &boxes);

  std::size_t size() const { return nodes.size(); }
};
} // namespace bvh
} // namespace spatial
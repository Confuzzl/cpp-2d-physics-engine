export module quadtree;

import <vector>;
import aabb;

export namespace spatial {
export namespace quadtree {
using node_index_t = int;

struct node_t {
  node_index_t children[4]{-1, -1, -1, -1};
};
struct tree_t {
  aabb_t bounds{};
  node_index_t root;
  std::vector<node_t> nodes{};
};
} // namespace quadtree
} // namespace spatial
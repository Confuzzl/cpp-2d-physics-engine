export module quadtree;

import <vector>;
import aabb;

export namespace spatial {
export namespace quadtree {
using index_t = int;

struct node_t {
  index_t children[4]{-1, -1, -1, -1};
};
struct tree_t {
  aabb_t bounds{};
  index_t root;
  std::vector<node_t> nodes{};
};
} // namespace quadtree
} // namespace spatial
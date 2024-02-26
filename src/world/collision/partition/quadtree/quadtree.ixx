export module quadtree;

import <vector>;
import aabb;

export namespace spatial {
export namespace quadtree {
using node_index_t = int;

struct node_t {
  node_index_t children[4]{-1, -1, -1, -1};
};
struct tree_t {};
} // namespace quadtree
} // namespace spatial
export module kdtree;

import glm;
import aabb;
import vector;

export namespace spatial {
namespace kd_tree {
using index_t = int;

template <typename vec_t = glm::vec2> struct tree_t {
  struct node_t {
    index_t parent = -1;
    index_t left = -1, right = -1;

    bool isLeaf() const { return left == -1 && right == -1; }

    const vec_t *pos;
    const gen_aabb_t<vec_t> *owner;

    node_t(const vec_t *pos, const gen_aabb_t<vec_t> *owner)
        : pos{pos}, owner{owner} {}
  };
  std::vector<node_t> nodes;

  tree_t() {}

  // const node_t &closestNode(const vec_t &point) const { return {}; }
};
} // namespace kd_tree
} // namespace spatial
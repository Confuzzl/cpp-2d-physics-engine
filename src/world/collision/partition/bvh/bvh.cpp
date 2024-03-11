module bvh;

spatial::bvh::tree_t::tree_t(const std::vector<aabb_t> &boxes) {
  nodes.reserve(boxes.size() * 2 - 1);
}
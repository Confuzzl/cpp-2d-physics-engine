export module screen;

import glm;
import aabb;

export namespace screen {
using unit_t = unsigned short;
using coord_t = glm::u16vec2;
using box = gen_aabb_t<coord_t>;
} // namespace screen
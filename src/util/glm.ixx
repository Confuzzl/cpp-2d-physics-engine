export module glm;

export import <glm/glm.hpp>;
export import <glm/gtc/matrix_transform.hpp>;
export import <glm/gtc/type_ptr.hpp>;
export import <glm/gtx/string_cast.hpp>;
export import <glm/gtx/matrix_interpolation.hpp>;
export import <glm/gtx/norm.hpp>;

// import <format>;

// export template <glm::length_t L, typename T, glm::qualifier Q>
// struct std::formatter<glm::vec<L, T, Q>> {
//   constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin();
//   } auto format(const glm::vec<L, T, Q> &vec, std::format_context &ctx) {
//     return std::format_to(ctx.out(), glm::to_string(vec));
//   }
// };

// export template <glm::length_t C, glm::length_t R, typename T, glm::qualifier
// Q> struct std::formatter<glm::mat<C, R, T, Q>> {
//   constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin();
//   } auto format(const glm::mat<C, R, T, Q> &mat, std::format_context &ctx) {
//     return std::format_to(ctx.out(), glm::to_string(mat));
//   }
// };
export module variant;

export import <variant>;

template <typename... branches_t> struct cases : branches_t... {
  using branches_t::operator()...;
};
template <typename... branches_t> cases(branches_t...) -> cases<branches_t...>;

template <typename variant_t, typename... branches_t>
auto match(variant_t &var, cases<branches_t...> &&matcher) {
  return std::visit(matcher, var);
}
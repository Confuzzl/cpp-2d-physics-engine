export module vector;

export import <vector>;

export template <typename T, typename... Args>
std::vector<T> vec_new(Args &&...args) {
  std::vector<T> out;
  out.reserve(sizeof...(Args));
  (out.emplace_back(std::forward<Args>(args)), ...);
  return out;
}
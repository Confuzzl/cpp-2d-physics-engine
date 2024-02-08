export module func;

import <vector>;
import <functional>;

export namespace func {
template <typename In, typename Out>
std::vector<Out> map(const std::vector<In> &in,
                     const std::function<Out(const In &)> &f) {
  std::vector<Out> out{};
  out.reserve(in.size());
  for (const In &i : in)
    out.emplace_back(f(i));
  return out;
}

template <typename In, typename Out>
std::vector<Out>
map2(const std::vector<In> &in,
     const std::function<Out(const In &, const int, const std::vector<In> &)>
         &f) {
  std::vector<Out> out{};
  out.reserve(in.size());
  for (int i = 0; i < in.size(); i++)
    out.emplace_back(f(in[i], i, in));
  return out;
}

const auto a =
    map2<int, double>(std::vector<int>{0, 1, 2, 3, 4},
                      [](const int &n, const int i, const std::vector<int> &v) {
                        return i / static_cast<double>(n);
                      });
} // namespace func
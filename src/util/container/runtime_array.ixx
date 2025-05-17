export module runtime_array;

import <memory>;
import <stdexcept>;
import <format>;

template <typename L, typename T> static void write(L &&list, T *dest) {
  std::move(list.begin(), list.end(), dest);
}
template <typename L, typename T> static void write(L &list, T *dest) {
  std::copy(list.begin(), list.end(), dest);
}

export template <typename T> struct runtime_array {
private:
  std::unique_ptr<T[]> array;
  std::size_t _size;

public:
  runtime_array() : _size{0} {}
  runtime_array(const std::size_t size)
      : array{std::make_unique<T[]>(size)}, _size{size} {}
  runtime_array(std::initializer_list<T> list)
      : array{std::make_unique<T[]>(list.size())}, _size{list.size()} {
    std::copy(list.begin(), list.begin() + _size, array.get());
  }
  runtime_array(std::unique_ptr<T[]> &&poly, const std::size_t size)
      : array{std::move(poly)}, _size{size} {}

  runtime_array(const runtime_array &that)
      : array{std::make_unique<T[]>(that._size)}, _size{that._size} {
    std::copy(that.array.get(), that.array.get() + _size, array.get());
  }
  runtime_array &operator=(const runtime_array &that) {
    array = std::make_unique<T[]>(that._size);
    _size = that._size;
    std::copy(that.array.get(), that.array.get() + _size, array.get());
    return *this;
  }
  runtime_array(runtime_array &&) = default;
  runtime_array &operator=(runtime_array &&) = default;

  T &operator[](const std::size_t i) { return *(array.get() + i); }
  const T &operator[](const std::size_t i) const { return *(array.get() + i); }

  T *begin() { return array.get(); }
  const T *begin() const { return array.get(); }
  const T *cbegin() const { return array.get(); }
  T *end() { return begin() + _size; }
  const T *end() const { return begin() + _size; }
  const T *cend() const { return cbegin() + _size; }

  T *data() { return array.get(); }
  const T *data() const { return array.get(); }

  std::size_t size() const { return _size; }
};
export template <typename T, typename L>
runtime_array<T> make_runtime_array(L &&list) {
  auto size = list.size();
  auto out = std::make_unique<T[]>(size);
  write(std::forward<L>(list), out.get());
  return {std::move(out), size};
}
// export template <typename T>
// runtime_array<T> make_runtime_array(const std::size_t size) {
//   auto out = std::make_unique<T[]>(size);
//   return {std::move(out), size};
// }

// export namespace func {
// template <typename In, typename Func>
// auto map(const runtime_array<In> &in, const Func &f) {
//   using Out = std::invoke_result_t<Func, In>;
//   static_assert(std::is_default_constructible_v<Out>,
//                 "Out must be default constructible");
//   runtime_array<Out> out{in.size()};
//   for (auto i = 0u; i < in.size(); i++)
//     out[i] = f(in[i]);
//   return out;
// }
//
// } // namespace func
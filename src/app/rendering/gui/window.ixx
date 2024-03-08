export module window;

import glm;
import <vector>;
import <memory>;

import screen;
import aabb;

export namespace gui {
struct window_t {
  screen::coord_t defaultPos;
  // screen::coord_t pos;
  // screen::coord_t size;
  screen::box box;

  window_t(const screen::coord_t &defaultPos, const screen::box &box);

  bool inBounds(const screen::coord_t &mousePos) const;
  bool onClick(const screen::coord_t &mousePos);

  void render() const;
  void renderTitleBar() const;

  void open();
  void close();
};
} // namespace gui
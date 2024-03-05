export module window;

import glm;
import <vector>;
import <memory>;

import screen;

export namespace gui {
struct window_t {
  screen::coord_t defaultPos;
  screen::coord_t pos;
  screen::coord_t size;

  window_t(const screen::coord_t &defaultPos, const screen::coord_t &size);

  bool inBounds(const screen::coord_t &mousePos) const;
  bool onClick(const screen::coord_t &mousePos);

  void render() const;
  void renderTitleBar() const;

  void open();
  void close();
};
} // namespace gui
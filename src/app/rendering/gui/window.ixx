export module window;

import glm;
import <vector>;
import <memory>;

import screen;

export namespace gui {

struct interactable_t {
  screen::coord_t pos;
  screen::coord_t size;

  interactable_t(const screen::coord_t &pos, const screen::coord_t &size);

  bool inBounds(const screen::coord_t &mousePos);
};

interactable_t *FOCUS = nullptr;

struct window_t : interactable_t {
  screen::coord_t defaultPos;

  // bool isOpen = false;

  // std::vector<std::unique_ptr<component>> components{};

  window_t(const screen::coord_t &defaultPos, const screen::coord_t &size);

  void render() const;
  void renderTitleBar() const;

  void open();
  void close();
};
} // namespace gui
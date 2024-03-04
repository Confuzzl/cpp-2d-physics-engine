export module window;

import glm;
import <vector>;
import <memory>;

export namespace gui {
using unit_t = unsigned short;
using coord_t = glm::u16vec2;

struct interactable_t {
  coord_t pos;
  coord_t size;

  interactable_t(const coord_t &pos, const coord_t &size);

  bool inBounds(const coord_t &mousePos);
};

interactable_t *FOCUS = nullptr;

struct window_t : interactable_t {
  coord_t defaultPos;

  // bool isOpen = false;

  // std::vector<std::unique_ptr<component>> components{};

  window_t(const coord_t &defaultPos, const coord_t &size);

  void render() const;
  void renderTitleBar() const;

  void open();
  void close();
};
} // namespace gui
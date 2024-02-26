export module window;

import glm;
import <vector>;
import <memory>;

export namespace gui {
// struct component {};
struct interactable {
  glm::u16vec2 pos;
  glm::u16vec2 size;

  bool inBounds(const glm::u16vec2 &v);
};

interactable *FOCUS = nullptr;

struct window {
  glm::u16vec2 defaultPos;
  glm::u16vec2 pos;
  glm::u16vec2 size;

  bool isOpen = false;

  // std::vector<std::unique_ptr<component>> components{};

  window(const glm::u16vec2 &defaultPos, const glm::u16vec2 &size);

  void render() const;
  void renderTitleBar() const;

  void open();
  void close();
};
} // namespace gui
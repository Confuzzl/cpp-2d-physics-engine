module window;

import rendering;
import color;
import debug;

gui::interactable_t::interactable_t(const screen::coord_t &pos,
                                    const screen::coord_t &size)
    : pos{pos}, size{size} {}
bool gui::interactable_t::inBounds(const screen::coord_t &mousePos) {
  return false;
}

gui::window_t::window_t(const screen::coord_t &defaultPos,
                        const screen::coord_t &size)
    : interactable_t(defaultPos, size), defaultPos{defaultPos} {}

void gui::window_t::render() const {
  renderTitleBar();
  MAIN_RENDERER.guiFrame.drawQuad(pos.x, pos.y, size.x, size.y, colors::GRAY);
}

void gui::window_t::renderTitleBar() const {
  static constexpr unsigned char BAR_HEIGHT = 30;
  MAIN_RENDERER.guiFrame.drawQuad(pos.x, pos.y + size.y, size.x, BAR_HEIGHT,
                                  colors::DARK_GRAY);
  MAIN_RENDERER.guiFrame.text("TEST", pos.x, pos.y + size.y);
}
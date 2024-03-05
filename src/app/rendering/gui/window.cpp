module window;

import rendering;
import color;
import debug;

gui::window_t::window_t(const screen::coord_t &defaultPos,
                        const screen::coord_t &size)
    : defaultPos{defaultPos}, pos{defaultPos}, size{size} {}

bool gui::window_t::inBounds(const screen::coord_t &mousePos) const {
  const screen::coord_t diff = mousePos - pos;
  // return 0 <= diff.x && diff.x <= size.x && 0 <= diff.y && diff.y <= size.y;
  return glm::all(glm::greaterThanEqual(diff, {})) &&
         glm::all(glm::lessThanEqual(diff, size));
}
bool gui::window_t::onClick(const screen::coord_t &mousePos) {
  if (!inBounds(mousePos))
    return false;
  return true;
}

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
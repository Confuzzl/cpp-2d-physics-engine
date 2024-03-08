module window;

import rendering;
import color;
import debug;

gui::window_t::window_t(const screen::coord_t &defaultPos,
                        const screen::box &box)
    : defaultPos{defaultPos}, box{box} {}

bool gui::window_t::inBounds(const screen::coord_t &mousePos) const {
  return box.contains(mousePos);
}
bool gui::window_t::onClick(const screen::coord_t &mousePos) {
  if (!inBounds(mousePos))
    return false;
  return true;
}

void gui::window_t::render() const {
  renderTitleBar();
  // MAIN_RENDERER.guiFrame.drawQuadToFrom(pos.x, pos.y, size.x, size.y,
  //                                       colors::GRAY);
  MAIN_RENDERER.guiFrame.drawQuadFromTo(box.min, box.max, colors::GRAY);
}

void gui::window_t::renderTitleBar() const {
  static constexpr unsigned char BAR_HEIGHT = 30;
  // MAIN_RENDERER.guiFrame.drawQuad(/*pos.x, pos.y + size.y, size.x,
  // BAR_HEIGHT,*/
  //                                 box.min, {box.width(), BAR_HEIGHT},
  //                                 colors::DARK_GRAY);
  MAIN_RENDERER.guiFrame.drawQuadFromTo(
      box.min, box.max + screen::coord_t{0, BAR_HEIGHT}, colors::DARK_GRAY);
  // MAIN_RENDERER.guiFrame.text("TEST", pos.x, pos.y + size.y);
}
module window;

import rendering;
import color;
import debug;

gui::window::window(const glm::u16vec2 &defaultPos, const glm::u16vec2 &size)
    : defaultPos{defaultPos}, pos{defaultPos}, size{size} {}

void gui::window::render() const {
  renderTitleBar();
  MAIN_RENDERER.guiFrame.drawQuad(pos.x, pos.y, size.x, size.y, colors::GRAY);
}

void gui::window::renderTitleBar() const {
  static constexpr unsigned char BAR_HEIGHT = 30;
  MAIN_RENDERER.guiFrame.drawQuad(pos.x, pos.y + size.y, size.x, BAR_HEIGHT,
                                  colors::DARK_GRAY);
  MAIN_RENDERER.guiFrame.text("TEST", pos.x, pos.y + size.y);
}
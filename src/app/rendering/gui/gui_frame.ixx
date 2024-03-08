module;

#include <ranges>

export module gui_frame;

import <vector>;
import <string>;

import color;

import aabb;
import glm;

export namespace gui {
export namespace font {
constexpr unsigned short TEXEL_RANGE = 1 << 15;
constexpr unsigned short IMG_WIDTH = 1024, IMG_HEIGHT = 1024;
constexpr unsigned char CHAR_WIDTH = 64, CHAR_HEIGHT = 128;
constexpr unsigned char COLUMNS = 16, ROWS = 8;
} // namespace font

struct window_t;

struct frame {
private:
  std::vector<window_t *> openWindows{};

public:
  auto topToBottom() {
    return openWindows |
           std::views::transform([](window_t *w) -> window_t & { return *w; });
  }
  auto topToBottom() const {
    return openWindows |
           std::views::transform(
               [](const window_t *w) -> const window_t & { return *w; });
  }
  auto bottomToTop() { return topToBottom() | std::views::reverse; }
  auto bottomToTop() const { return topToBottom() | std::views::reverse; }

  void handleClick();

  frame();
  ~frame();

  void render() const;

  void text(const std::string &str, const unsigned short x = 0,
            const unsigned short y = 0, const double scale = 0.25) const;
  void drawQuad(const glm::u8vec2 &pos, const glm::u8vec2 &size,
                const color_t &color = colors::WHITE) const {
    drawQuad(pos.x, pos.y, size.x, size.y, color);
  }
  void drawQuad(const unsigned short x, const unsigned short y,
                const unsigned short width, const unsigned short height,
                const color_t &color = colors::WHITE) const;
  void drawQuadFromTo(const unsigned short x1, const unsigned short y1,
                      const unsigned short x2, const unsigned short y2,
                      const color_t &color = colors::WHITE) const;
  void drawQuadFromTo(const glm::u8vec2 &min, const glm::u8vec2 &max,
                      const color_t &color = colors::WHITE) const {
    drawQuadFromTo(min.x, min.y, max.x, max.y, color);
  }
  // template <typename vec_t>
  // void drawAABB(const gen_aabb_t<vec_t> &aabb,
  //               const color_t &color = colors::WHITE) const {
  //   drawQuadFromTo(static_cast<unsigned short>(aabb.min.x),
  //                  static_cast<unsigned short>(aabb.min.y),
  //                  static_cast<unsigned short>(aabb.max.x),
  //                  static_cast<unsigned short>(aabb.max.y), color);
  // }
};
} // namespace gui
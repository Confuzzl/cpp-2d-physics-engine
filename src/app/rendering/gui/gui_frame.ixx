module;

#include <ranges>

export module gui_frame;

import <vector>;
import <string>;

import color;

export namespace gui {
export namespace font {
constexpr unsigned short TEXEL_RANGE = 1 << 15;
constexpr unsigned short IMG_WIDTH = 1024, IMG_HEIGHT = 1024;
constexpr unsigned char CHAR_WIDTH = 64, CHAR_HEIGHT = 128;
constexpr unsigned char COLUMNS = 16, ROWS = 8;
} // namespace font

struct window;

struct frame {
private:
  std::vector<window *> openWindows{};

public:
  auto topToBottom() const {
    return openWindows |
           std::views::transform(
               [](const window *w) -> const window & { return *w; });
  }
  auto bottomToTop() const { return topToBottom() | std::views::reverse; }

  frame();
  ~frame();

  void render() const;

  void text(const std::string &str, const unsigned short x = 0,
            const unsigned short y = 0, const double scale = 0.25) const;
  void drawQuad(const unsigned short x, const unsigned short y,
                const unsigned short width, const unsigned short height,
                const color_t &color = colors::WHITE) const;
  void drawQuadFromTo(const unsigned short x1, const unsigned short y1,
                      const unsigned short x2, const unsigned short y2,
                      const color_t &color = colors::WHITE) const;
};
} // namespace gui
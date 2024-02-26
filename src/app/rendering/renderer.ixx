export module rendering;

// import <string>;
import glm;

import gui_frame;
import world_frame;
// import color;

export struct Renderer {
  static const glm::mat4 UI_MATRIX;

  // shader::holder shaders{};
  // tex_storage textures{};
  gui::frame guiFrame{};
  world::frame worldFrame{};

  Renderer();
  /*call after glfwinit*/
  void init();

  void renderFrame(const double t) const;
  // void renderScene(const double t) const;
  // void renderText(const double t) const;

  // void text(const std::string &str, const unsigned short x = 0,
  //           const unsigned short y = 0, const double scale = 0.25) const;

  /*void render(const Object &object, render_opts &&opts) const;
  void render(const AABB &aabb, const glm::uvec3 &color) const;

  void drawGrid() const;

  void drawPoint(const glm::vec2 &point, const float size = 10,
                 const color_t &color = colors::WHITE) const;
  void drawPointFixed(const glm::vec2 &point, const float size = 10,
                      const color_t &color = colors::WHITE) const;

  void drawLine(const glm::vec2 &a, const glm::vec2 &b, const float size = 5,
                const color_t &color = colors::WHITE) const;
  void drawLineFixed(const glm::vec2 &a, const glm::vec2 &b,
                     const float size = 5,
                     const color_t &color = colors::WHITE) const;

  void drawArrow(const glm::vec2 &origin, const glm::vec2 &vector,
                 const color_t &color = colors::WHITE) const;
  void drawArrowFromTo(const glm::vec2 &from, const glm::vec2 &to,
                       const color_t &color = colors::WHITE) const;

  void drawCircle(const glm::vec2 &center, const float radius,
                  const color_t &color = colors::WHITE) const;

  void drawBox(const glm::vec2 &start, const glm::vec2 &size,
               const float lineSize = 5,
               const color_t &color = colors::WHITE) const;
  void drawBoxFromTo(const glm::vec2 &from, const glm::vec2 &to,
                     const float lineSize = 5,
                     const color_t &color = colors::WHITE) const;
  void drawBoxFixed(const glm::vec2 &start, const glm::vec2 &size,
                    const float lineSize = 5,
                    const color_t &color = colors::WHITE) const;
  void drawBoxFromToFixed(const glm::vec2 &from, const glm::vec2 &to,
                          const float lineSize = 5,
                          const color_t &color = colors::WHITE) const;

  void drawQuad(const glm::vec2 &start, const glm::vec2 &size,
                const color_t &color = colors::WHITE) const;
  void drawQuadFromTo(const glm::vec2 &from, const glm::vec2 &to,
                      const color_t &color = colors::WHITE) const;*/
} MAIN_RENDERER;
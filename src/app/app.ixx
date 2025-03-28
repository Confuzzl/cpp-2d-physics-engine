module;

#include "util/gl.h"

export module app;

import <memory>;

import update_cycle;
import scene;
import rendering;
import window;
import glm;
// import aabb;

export struct App {
  static constexpr GLsizei WIDTH = 1280, HEIGHT = 720;
  static constexpr glm::uvec2 DIMENSIONS{WIDTH, HEIGHT};
  static constexpr glm::mat4 UI_MATRIX = glm::ortho(WIDTH, HEIGHT);
  static constexpr float ASPECT_RATIO = static_cast<float>(WIDTH) / HEIGHT;

  // Must initialize first in order to load GLFW and glad
  Window window;

  UpdateCycle loopCycle;
  UpdateCycle inputCycle;
  UpdateCycle updateCycle;
  UpdateCycle frameCycle;
  unsigned int seconds = 0;

  Renderer renderer;

  Scene scene;

  App();
  ~App();

  void start();
  void startUpdate(const double t);
  // void processInput(const double dt);
  void startFrame(const double t);

  void test();

  void close();

  glm::vec2 cursorPosition();
  glm::vec2 cursorGUIPosition();
  glm::vec2 cursorWorldPosition();
} MAIN_APP{};

module;

#include "util/gl.h"
#include "util/main_objects.h"
#include <stdexcept>

module app;

import input_handler;
import debug;
import gl_debug;
import rendering;

// const glm::vec2 App::BoundingBoxS{App::WIDTH, App::HEIGHT};

App::App() try : loopCycle{0}, inputCycle{60}, updateCycle{60}, frameCycle{60} {
  glfwSetCursorPosCallback(window, InputHandler::mousePosCallback);
  glfwSetMouseButtonCallback(window, InputHandler::mouseClickCallback);
  glfwSetScrollCallback(window, InputHandler::scrollCallback);
  glfwSetKeyCallback(window, InputHandler::keyCallback);

  glPolygonMode(GL_FRONT, GL_FILL);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_CULL_FACE);

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(debugCallback, 0);
} catch (const std::runtime_error &e) {
  println("APP INITIALIZATION ERROR:\n{}", e.what());
}
App::~App() {
  println("app terminated at {:.2f}s", glfwGetTime());
  println("min: {}\navg: {}\nmax: {}", MAIN_RENDERER.minElapsed / 1'000'000.0,
          (MAIN_RENDERER.elapsedAccumulate / MAIN_RENDERER.elapsedCounter) /
              1'000'000.0,
          MAIN_RENDERER.maxElapsed / 1'000'000.0);
  glfwDestroyWindow(window);
  glfwTerminate();
}

void App::start() {
  MAIN_SCENE.init();

  updateCycle.toggle();

  while (!glfwWindowShouldClose(window)) {
    const double currentTime = glfwGetTime();

    loopCycle.pushNewTime(currentTime);
    if (inputCycle.isPastLength(currentTime)) {
      inputCycle.pushNewTime(currentTime);
      InputHandler::processInput(inputCycle.dt);
    }
    if (updateCycle.isPastLength(currentTime))
      startUpdate(currentTime);
    if (frameCycle.isPastLength(currentTime))
      startFrame(currentTime);
    if (currentTime - seconds >= 1) {
      loopCycle.pushCount();
      updateCycle.pushCount();
      frameCycle.pushCount();
      seconds++;
    }
  }
}

void App::startUpdate(const double t) {
  updateCycle.pushNewTime(t);
  if (!updateCycle.locked)
    MAIN_SCENE.update(updateCycle.dt);
}
void App::startFrame(const double t) {
  frameCycle.pushNewTime(t);
  MAIN_RENDERER.renderFrame(t);
}

void App::close() { glfwSetWindowShouldClose(window, true); }

import physics;

void App::test() { physics::test(); }

glm::vec2 App::cursorPosition() {
  double x, y;
  glfwGetCursorPos(window, &x, &y);
  return {x, y};
}
glm::vec2 App::cursorGUIPosition() {
  auto cursorPos = cursorPosition();
  cursorPos.y = App::HEIGHT - cursorPos.y;
  return cursorPos;
}
glm::vec2 App::cursorWorldPosition() {
  auto cursorPos = cursorPosition();
  cursorPos -= App::DIMENSIONS / 2u;
  cursorPos.y *= -1;
  cursorPos /= App::HEIGHT / 2;
  cursorPos /= MAIN_CAMERA.zoom();
  cursorPos += MAIN_CAMERA.getPos();
  return cursorPos;
}
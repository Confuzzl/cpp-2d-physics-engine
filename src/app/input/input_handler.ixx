module;

#include "gl.h"

export module input_handler;

import key;

import glm;

import <map>;
import <functional>;

export struct InputHandler {
  static std::map<int, Key> keys;

  static void processInput(const double dt);

  static void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                          int mods);
  static void mouseCallback(GLFWwindow *window, double xpos, double ypos);
  static void scrollCallback(GLFWwindow *window, double xpos, double ypos);
};
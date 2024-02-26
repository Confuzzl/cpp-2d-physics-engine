module;

#include "util/gl.h"
// #define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

module cursor;

import <format>;

void CursorHolder::init() {
  ARROW = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
  HAND = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
  IBEAM = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
  HRESIZE = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
  VRESIZE = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);

  loadCustomCursor("grab_open", GRAB_OPEN, 9, 9);
  loadCustomCursor("grab_close", GRAB_CLOSE, 9, 9);
  loadCustomCursor("move", MOVE, 11, 11);
}
void CursorHolder::loadCustomCursor(const std::string &name,
                                    GLFWcursor *&cursor, const int xhot,
                                    const int yhot) {
  const std::string path = "assets/cursor/" + name + ".png";
  stbi_set_flip_vertically_on_load(false);
  int width, height, channels;
  unsigned char *data =
      stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
  GLFWimage img;
  img.width = width;
  img.height = height;
  img.pixels = data;
  cursor = glfwCreateCursor(&img, xhot, yhot);

  stbi_image_free(data);
}

CursorHolder::~CursorHolder() {
  glfwDestroyCursor(ARROW);
  glfwDestroyCursor(HAND);
  glfwDestroyCursor(GRAB_OPEN);
  glfwDestroyCursor(GRAB_CLOSE);
  glfwDestroyCursor(MOVE);
  glfwDestroyCursor(IBEAM);
  glfwDestroyCursor(HRESIZE);
  glfwDestroyCursor(VRESIZE);
}
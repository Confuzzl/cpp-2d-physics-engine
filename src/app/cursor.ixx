module;

#include "util/gl.h"

export module cursor;

import <string>;

export struct CursorHolder {
  GLFWcursor *ARROW;
  GLFWcursor *HAND;
  GLFWcursor *GRAB_OPEN, *GRAB_CLOSE;
  GLFWcursor *MOVE;
  GLFWcursor *IBEAM;
  GLFWcursor *HRESIZE, *VRESIZE;

  void init();

  void loadCustomCursor(const std::string &name, GLFWcursor *&cursor,
                        const int xhot, const int yhot);

  ~CursorHolder();
};
module;

#include "gl.h"

export module key;

import glm;
import app;

import <functional>;

export struct Key {
  using Callback = std::function<void(const double)>;

  static const Callback NONE;

  bool on = false;
  bool justUsed = false;

  const Callback processJustOn;
  const Callback processOn;
  const Callback processJustOff;
  const Callback processOff;

  Key(const Callback &processJustOn, const Callback &processOn,
      const Callback &processJustOff, const Callback &processOff)
      : processJustOn{processJustOn}, processOn{processOn},
        processJustOff{processJustOff}, processOff{processOff} {}
  Key(const Callback &processOn, const Callback &processOff)
      : Key(processOn, processOn, processOff, processOff){};
  Key(const Callback &processOn) : Key(processOn, NONE) {}

  void change(int action) {
    switch (action) {
    case GLFW_RELEASE: {
      on = false;
      justUsed = true;
      break;
    }
    case GLFW_PRESS: {
      on = true;
      justUsed = true;
      break;
    }
    case GLFW_REPEAT: {
    }
    }
  }

  void operator()(const double dt) {
    if (on) {
      if (justUsed) {
        processJustOn(dt);
        justUsed = false;
      } else {
        processOn(dt);
      }
    } else {
      if (justUsed) {
        processJustOff(dt);
        justUsed = false;
      } else {
        processOff(dt);
      }
    }
  }

  static Callback moveFunction(const glm::vec2 &direction) {
    return [direction](const double dt) {
      MAIN_CAMERA.translate(direction *
                            static_cast<float>(dt * MAIN_CAMERA.speed));
    };
  }
};

const Key::Callback Key::NONE{[](const double) {}};
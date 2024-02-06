export module key;

import glm;
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
      const Callback &processJustOff, const Callback &processOff);
  Key(const Callback &processOn, const Callback &processOff);
  Key(const Callback &processOn);

  void change(int action);

  void operator()(const double dt);

  static Callback moveFunction(const glm::vec2 &direction);
};
const Key::Callback Key::NONE{[](const double) {}};
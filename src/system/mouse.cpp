#include <termui/system/mouse.hpp>

namespace termui {

MouseInteraction::MouseInteraction(std::string s) : valid(true) {
  int  code, x, y;
  char mstate;

  if (sscanf(s.data(), "\x1b[<%d;%d;%d%c", &code, &x, &y, &mstate) != 4) {
    valid = false;
    return;
  }

  col = x;
  row = y;

  // decode modifiers from code
  modifiers.shift = (code & 4) != 0;
  modifiers.option = (code & 8) != 0;
  modifiers.control = (code & 16) != 0;

  // decode button/event from low bits
  switch (code & 3) {
  case 0:
    button = MouseButton::Left;
    break;
  case 1:
    button = MouseButton::Middle;
    break;
  case 2:
    button = MouseButton::Right;
    break;
  case 3:
    button = MouseButton::None;
    break;
  }

  // decode event type
  if (mstate == 'm') {
    event = EventType::ButtonRelease;

  } else if (code & 32) {
    event = EventType::Move;

  } else if (code == 64) {
    event = EventType::ScrollUp;
    button = MouseButton::None; // scrolling codes include a button for some reason 0_0

  } else if (code == 65) {
    event = EventType::ScrollDown;
    button = MouseButton::None;

  } else {
    event = EventType::ButtonPress;
  }
}

bool MouseInteraction::match(EventType e, MouseButton b, ModifierKey m) {
  if (!valid)
    return false;
  if (event != e)
    return false;
  if (button != b)
    return false;
  if (modifiers != m)
    return false;

  return true;
}

bool MouseInteraction::match(size_t x1, size_t x2, size_t y1, size_t y2, EventType e, MouseButton b, ModifierKey m) {
  if (!valid)
    return false;
  if (event != e)
    return false;
  if (button != b)
    return false;
  if (modifiers != m)
    return false;
  if (col < x1 || col > x2)
    return false;
  if (row < y1 || row > y2)
    return false;

  return true;
}

} // namespace termui

#ifndef MOUSE_HPP
#define MOUSE_HPP

#include <string>
#include <termios.h>

namespace termui {

enum class EventType {
  Move,
  ScrollUp,
  ScrollDown,
  ButtonPress,
  ButtonRelease,
};

enum class MouseButton {
  None,
  Left,
  Middle,
  Right,
};

struct ModifierKey {
  bool shift = false;
  bool control = false;
  bool option = false;

  bool operator==(const ModifierKey &) const = default;
};

namespace Modifiers {
inline constexpr ModifierKey None = {};
inline constexpr ModifierKey Shift = {.shift = true};
inline constexpr ModifierKey Control = {.control = true};
inline constexpr ModifierKey Option = {.option = true};
} // namespace Modifiers

struct MouseInteraction {
  bool valid; // indicates whether class was constructed using valid SGR code

  size_t      col, row; // on (all ?) terminals, origin (1,1) is top right
  EventType   event;
  MouseButton button;
  ModifierKey modifiers;

  MouseInteraction(std::string s); // constructor decodes the string

  // match methods detect if a certain mouse event takes place in a region box
  // check implementation or docs to understand how bounds are defined
  bool match(EventType event, MouseButton button = MouseButton::None, ModifierKey modifiers = Modifiers::None);
  bool match(size_t x1, size_t x2, size_t y1, size_t y2, EventType event, MouseButton button = MouseButton::None, ModifierKey modifiers = Modifiers::None);
};

} // namespace termui

#endif

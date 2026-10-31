#include <termui/core/color.hpp>

namespace termui {

Color::Color() : mode(Mode::Inherit), value(0) {}
Color::Color(uint8_t v) : mode(Mode::Ansi256), value(v) {}
Color::Color(uint8_t r, uint8_t g, uint8_t b) : mode(Mode::TrueColor), rgb{r, g, b} {}
Color::Color(std::string hex) : mode(Mode::TrueColor), rgb{0, 0, 0} {
  rgb.r = static_cast<uint8_t>(std::stoi(hex.substr(1, 2), nullptr, 16));
  rgb.g = static_cast<uint8_t>(std::stoi(hex.substr(3, 2), nullptr, 16));
  rgb.b = static_cast<uint8_t>(std::stoi(hex.substr(5, 2), nullptr, 16));
}

Color Color::Reset() {
  Color c;
  c.mode = Mode::Reset;
  return c;
}

Color Color::Inherit() {
  return Color{}; // same as default
}

bool Color::operator==(const Color &other) const {
  if (mode != other.mode)
    return false;

  switch (mode) {
  case Mode::TrueColor:
    return rgb.r == other.rgb.r && rgb.g == other.rgb.g && rgb.b == other.rgb.b;
  case Mode::Ansi256:
    return value == other.value;
  case Mode::Inherit:
  case Mode::Reset:
    return mode == other.mode;
  }
  return false; // unreachable
}

bool Color::IsInherit() const { return mode == Mode::Inherit; }
bool Color::IsReset() const { return mode == Mode::Reset; }

bool Color::isDark() const { // should only work for trucol
  // if (mode != Mode::truecolor) {
  //   return;
  // }

  auto lum     = (0.299 * rgb.r) + (0.587 * rgb.g) + (0.114 * rgb.b);
  bool is_dark = lum < 127.5;

  return is_dark;
}

bool Color ::isLight() const { return !isDark(); }

} // namespace termui

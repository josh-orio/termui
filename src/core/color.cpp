#include <termui/core/color.hpp>

namespace termui {

Color::Color() : mode(Mode::INHERIT), value(0) {}
Color::Color(uint8_t v) : mode(Mode::ANSI256), value(v) {}
Color::Color(uint8_t r, uint8_t g, uint8_t b) : mode(Mode::TRUECOLOR), rgb{r, g, b} {}
Color::Color(std::string hex) : mode(Mode::TRUECOLOR), rgb{0, 0, 0} {
  rgb.r = static_cast<uint8_t>(std::stoi(hex.substr(1, 2), nullptr, 16));
  rgb.g = static_cast<uint8_t>(std::stoi(hex.substr(3, 2), nullptr, 16));
  rgb.b = static_cast<uint8_t>(std::stoi(hex.substr(5, 2), nullptr, 16));
}

Color Color::Reset() {
  Color c;
  c.mode = Mode::RESET;
  return c;
}

Color Color::Inherit() {
  return Color{}; // same as default
}

bool Color::operator==(const Color &other) const {
  if (mode != other.mode)
    return false;

  switch (mode) {
  case Mode::TRUECOLOR:
    return rgb.r == other.rgb.r && rgb.g == other.rgb.g && rgb.b == other.rgb.b;
  case Mode::ANSI256:
    return value == other.value;
  case Mode::INHERIT:
  case Mode::RESET:
    return mode == other.mode;
  }
  return false; // unreachable
}

bool Color::IsInherit() const { return mode == Mode::INHERIT; }
bool Color::IsReset() const { return mode == Mode::RESET; }

} // namespace termui

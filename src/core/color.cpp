#include <termui/core/color.hpp>

namespace termui {

Color::Color(uint8_t v) : value(v), mode(ColorMode::ANSI256) {}
Color::Color(uint8_t r, uint8_t g, uint8_t b) : rgb(r, g, b), mode(ColorMode::TRUECOLOR) {}

bool Color::operator==(const Color &other) const {
  if (mode != other.mode)
    return false;

  if (mode == ColorMode::TRUECOLOR) {
    return rgb.r == other.rgb.r && rgb.g == other.rgb.g && rgb.b == other.rgb.b;

  } else if (mode == ColorMode::ANSI256) {
    return value == other.value;

  } else if (mode == ColorMode::ASCII) {
    return true;

  } else {
    return false; // not possible really
  }
}

} // namespace termui

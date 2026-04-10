#ifndef COLOR_HPP
#define COLOR_HPP

#include <cstdint>

namespace termui {

enum class ColorMode { ASCII, ANSI256, TRUECOLOR };

struct Color {
  union {
    uint8_t value;
    struct {
      uint8_t r, g, b;
    } rgb;
  };

  ColorMode mode;

  Color(uint8_t v);
  Color(uint8_t r, uint8_t g, uint8_t b);

  bool operator==(const Color &other) const;
};

namespace Colors {
inline termui::Color Red{9};
inline termui::Color Green{10};
inline termui::Color Yellow{3};
inline termui::Color Blue{4};
inline termui::Color Magenta{5};
inline termui::Color Cyan{6};
inline termui::Color White{231};
inline termui::Color Black{232};
inline Color         Lavender(122, 127, 167);
inline Color         Orange(222, 121, 92);
inline Color         Rose(212);
} // namespace Colors

} // namespace termui

#endif

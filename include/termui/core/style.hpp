#ifndef STYLE_HPP
#define STYLE_HPP

#include <map>
#include <optional>
#include <termui/core/color.hpp>

namespace termui {

enum class SGR : uint8_t {
  None = 0,
  Bold = 1,
  Faint = 2,
  Italic = 3,
  Underlined = 4,
  Blink = 5,
  FastBlink = 6, // implementation is 50/50 on different terms
  ReverseVideo = 7,
  Conceal = 8,
  CrossedOut = 9, // here too
};

class Style {
public:
  Style();
  Style(std::optional<Color> fg, std::optional<Color> bg);
  Style(std::optional<Color> fg, std::optional<Color> bg, std::vector<SGR> sgr);

  const std::optional<Color> &Foreground() const;
  const std::optional<Color> &Background() const;
  const std::vector<SGR>     &Attributes() const;

  bool operator==(const Style &other) const;
  bool operator!=(const Style &other) const;

private:
  std::optional<Color> foreground_color, background_color;
  std::vector<SGR>     attributes;
};

class StyleMap {
public:
  StyleMap(std::map<std::string, termui::Style> styles);

  const Style &at(std::string style);

private:
  std::map<std::string, Style> styles;
};

// }
//   enum class ElementState { Scoped, Unscoped, Disabled };
//   std::map<ElementState, Style> styles;

//   const Style &for_state(ElementState es) { return styles[es]; }
// }; TODO: implement styles based on widget state

namespace Styles {
inline termui::Style none(std::nullopt, std::nullopt, {});
inline termui::Style bold(std::nullopt, std::nullopt, {SGR::Bold});
inline termui::Style faint(std::nullopt, std::nullopt, {SGR::Faint});
}; // namespace Styles

} // namespace termui

#endif

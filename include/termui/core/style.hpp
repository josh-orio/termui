#ifndef STYLE_HPP
#define STYLE_HPP

#include <map>
#include <optional>
#include <string>
#include <termui/core/color.hpp>
#include <vector>

namespace termui {

enum class SGR : uint8_t {
  Inherit = uint8_t(-1), // 255 ig?
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
  Style(Color fg, Color bg);
  Style(Color fg, Color bg, std::vector<SGR> sgr);
  Style(std::vector<SGR> sgr);

  const Color            &Foreground() const;
  const Color            &Background() const;
  const std::vector<SGR> &Attributes() const;

  Style &Foreground(Color);
  Style &Background(Color);
  Style &Attributes(std::vector<SGR>);

  bool operator==(const Style &other) const;
  bool operator!=(const Style &other) const;

  // bool IsNone() const;
  // bool NotNone() const;

  bool InheritsSGR() const;
  bool ResetsSGR() const;

private:
  Color            foreground_color = Color::Inherit();
  Color            background_color = Color::Inherit();
  std::vector<SGR> attributes;
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
inline termui::Style none(Color::Reset(), Color::Reset(), {SGR::None}); // resets like \x1b[0m
inline termui::Style inherit(Color::Inherit(), Color::Inherit());
inline termui::Style bold({SGR::Bold});
inline termui::Style faint({SGR::Faint});
}; // namespace Styles

} // namespace termui

namespace tui {
using stl = termui::Style;
namespace stls = termui::Styles;
using sgr = termui::SGR;
} // namespace tui

#endif

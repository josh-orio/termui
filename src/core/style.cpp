#include <termui/core/style.hpp>

namespace termui {

// --- Style ---
Style::Style() : foreground_color(std::nullopt), background_color(std::nullopt), attributes() {}
Style::Style(std::optional<Color> fg, std::optional<Color> bg) : foreground_color(fg), background_color(bg), attributes() {}
Style::Style(std::optional<Color> fg, std::optional<Color> bg, std::vector<SGR> sgr) : foreground_color(fg), background_color(bg), attributes(sgr) {}

const std::optional<Color> &Style::Foreground() const { return foreground_color; }
const std::optional<Color> &Style::Background() const { return background_color; }
const std::vector<SGR>     &Style::Attributes() const { return attributes; }

bool Style::operator==(const Style &other) const {
  return foreground_color == other.foreground_color && background_color == other.background_color && attributes == other.attributes;
}

bool Style::operator!=(const Style &other) const { return !(*this == other); }

// --- StyleMap ---
StyleMap::StyleMap(std::map<std::string, termui::Style> styles) : styles(styles) {}

const Style &StyleMap::at(std::string style) { return styles[style]; }

} // namespace termui

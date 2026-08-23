#include <termui/core/style.hpp>

namespace termui {

// --- Style ---

Style::Style() : foreground_color(Color::Inherit()), background_color(Color::Inherit()), attributes({SGR::Inherit}) {}
Style::Style(Color fg, Color bg) : foreground_color(fg), background_color(bg), attributes({SGR::Inherit}) {}
Style::Style(Color fg, Color bg, std::vector<SGR> sgr) : foreground_color(fg), background_color(bg), attributes(sgr) {}
Style::Style(std::vector<SGR> sgr) : foreground_color(Color::Inherit()), background_color(Color::Inherit()), attributes(sgr) {}

const Color            &Style::Foreground() const { return foreground_color; }
const Color            &Style::Background() const { return background_color; }
const std::vector<SGR> &Style::Attributes() const { return attributes; }

Style &Style::Foreground(Color c) {
  foreground_color = c;
  return *this;
}

Style &Style::Background(Color c) {
  background_color = c;
  return *this;
}

Style &Style::Attributes(std::vector<SGR> a) {
  attributes = a;
  return *this;
}

bool Style::operator==(const Style &other) const {
  return foreground_color == other.foreground_color && background_color == other.background_color && attributes == other.attributes;
}
bool Style::operator!=(const Style &other) const { return !(*this == other); }

bool Style::InheritsSGR() const { return attributes.size() == 1 && attributes.at(0) == SGR::Inherit; }

bool Style::ResetsSGR() const { return attributes.size() == 1 && attributes.at(0) == SGR::None; }

// --- StyleMap ---
StyleMap::StyleMap(std::map<std::string, termui::Style> styles) : styles(styles) {}

const Style &StyleMap::at(std::string style) { return styles[style]; }

} // namespace termui

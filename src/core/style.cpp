#include <termui/core/style.hpp>

namespace termui {

// --- Style ---

Style::Style() : foregroundColor(Color::Inherit()), backgroundColor(Color::Inherit()), attributes({SGR::Inherit}) {}
Style::Style(Color fg, Color bg) : foregroundColor(fg), backgroundColor(bg), attributes({SGR::Inherit}) {}
Style::Style(Color fg, Color bg, std::vector<SGR> sgr) : foregroundColor(fg), backgroundColor(bg), attributes(sgr) {}
Style::Style(std::vector<SGR> sgr) : foregroundColor(Color::Inherit()), backgroundColor(Color::Inherit()), attributes(sgr) {}

bool Style::operator==(const Style &other) const {
  return foregroundColor == other.foregroundColor && backgroundColor == other.backgroundColor && attributes == other.attributes;
}
bool Style::operator!=(const Style &other) const { return !(*this == other); }

bool Style::InheritsSGR() const { return attributes.size() == 1 && attributes.at(0) == SGR::Inherit; }

bool Style::ResetsSGR() const { return attributes.size() == 1 && attributes.at(0) == SGR::None; }

// --- StyleMap ---
StyleMap::StyleMap(std::map<std::string, termui::Style> styles) : styles(styles) {}

void StyleMap::add(std::string key, Style style) { styles[key] = style; }

const Style &StyleMap::at(std::string style) { return styles[style]; }

std::vector<std::string> StyleMap::keys() {
  std::vector<std::string> keys;

  for (auto const &s : styles) {
    keys.push_back(s.first);
  }

  return keys;
}

} // namespace termui

#ifndef LINE_HPP
#define LINE_HPP

#include <string>
#include <termui/core/style.hpp>
#include <termui/globals.hpp>

namespace termui {

class Line {
public:
  enum class Direction { Horizontal, Vertical, Ascending, Descending };

  // Ascending (╱) and Descending (╲) are diagonal lines, drawn using the example characters.

  Line(Direction d, size_t len, std::string symbol = "");

  std::string render();

private:
  Direction   direction;
  size_t      len;
  std::string symbol;
};

} // namespace termui

namespace tui {
using line = termui::Line;

namespace linedir {
constexpr auto h = termui::Line::Direction::Horizontal;
constexpr auto v = termui::Line::Direction::Vertical;
constexpr auto a = termui::Line::Direction::Ascending;
constexpr auto d = termui::Line::Direction::Descending;
} // namespace linedir

} // namespace tui

#endif

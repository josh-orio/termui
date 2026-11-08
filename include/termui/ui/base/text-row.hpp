#ifndef TEXTROW_HPP
#define TEXTROW_HPP

#include <termui/ui/base/text.hpp>

#include <optional>
#include <utility>
#include <vector>

namespace termui {

// Text segments laid out side by side as columns sharing one width, filled left to right
class TextRow {
public:
  TextRow(std::optional<unsigned int> width = std::nullopt, std::optional<unsigned int> height = 1);

  TextRow &add(const Text &text, unsigned int reserve = 0); // reserve is held back from earlier segments
  TextRow &width(std::optional<unsigned int> w);
  TextRow &height(std::optional<unsigned int> h); // unset fits the tallest segment
  TextRow &size(std::optional<unsigned int> w, std::optional<unsigned int> h);
  TextRow &padding(unsigned int p); // empty cells between segments, defaults to 1

  std::pair<unsigned int, unsigned int> size(); // measures and returns the dimensions of the rendered row

  void render();

private:
  struct Segment {
    Text         text;
    unsigned int reserve;
  };

  static std::vector<std::string> natural_lines(Text text); // lines of a segment, ignoring any size set on it

  std::vector<std::pair<unsigned int, unsigned int>> layout(); // padding before and width of each segment
  unsigned int                                       rows();   // height shared by every segment

  std::vector<Segment>        _segments;
  std::optional<unsigned int> _w, _h;
  unsigned int                _padding = 1;
};

} // namespace termui

#endif

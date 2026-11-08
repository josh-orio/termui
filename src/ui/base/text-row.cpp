#include <termui/ui/ui.hpp>

namespace termui {

TextRow::TextRow(std::optional<unsigned int> width, std::optional<unsigned int> height) : _w(width), _h(height) {}

TextRow &TextRow::add(const Text &text, unsigned int reserve) {
  _segments.push_back({text, reserve});
  return *this;
}

TextRow &TextRow::width(std::optional<unsigned int> w) {
  _w = w;
  return *this;
}

TextRow &TextRow::height(std::optional<unsigned int> h) {
  _h = h;
  return *this;
}

TextRow &TextRow::size(std::optional<unsigned int> w, std::optional<unsigned int> h) {
  _w = w;
  _h = h;
  return *this;
}

TextRow &TextRow::padding(unsigned int p) {
  _padding = p;
  return *this;
}

std::vector<std::string> TextRow::natural_lines(Text text) { return text.size(std::nullopt, std::nullopt).layout(); }

unsigned int TextRow::rows() {
  if (_h) {
    return *_h;
  }

  size_t tallest = 0;

  for (Segment &seg : _segments) {
    tallest = std::max(tallest, natural_lines(seg.text).size());
  }

  return tallest;
}

std::vector<std::pair<unsigned int, unsigned int>> TextRow::layout() {
  std::vector<std::pair<unsigned int, unsigned int>> cols; // padding before, width
  bool                                               drawn = false; // padding only goes between drawn segments

  for (Segment &seg : _segments) {
    unsigned int widest = 0;

    for (const std::string &line : natural_lines(seg.text)) {
      widest = std::max<unsigned int>(widest, visible_length(line));
    }

    const unsigned int seg_w = std::max(widest, seg.reserve);

    cols.push_back({(drawn && seg_w > 0) ? _padding : 0, seg_w});
    drawn = drawn || seg_w > 0;
  }

  if (!_w) {
    return cols;
  }

  // reserves are taken from the right, so the segment that runs out of room is cut with an ellipsis
  std::vector<int> held(_segments.size(), 0); // width reserved by the segments after each one, with their padding
  int              total = 0;

  for (int i = static_cast<int>(_segments.size()) - 1; i >= 0; i--) {
    held[i] = total;

    if (_segments[i].reserve > 0) {
      total = std::min<int>(total + _segments[i].reserve + _padding, *_w);
    }
  }

  int remaining = *_w;
  drawn         = false;

  for (int i = 0; i < _segments.size(); i++) {
    const bool is_last = i == _segments.size() - 1;
    auto &[pad, seg_w] = cols[i];

    pad = drawn ? _padding : 0;

    if (is_last) { // leftover space goes to the last segment, so a right aligned one stays against the right edge
      pad   = std::min<int>(pad, remaining);
      seg_w = remaining - pad;
    }
    else {
      const int avail = std::max(0, remaining - held[i]);

      seg_w = avail > static_cast<int>(pad) ? std::min<int>(seg_w, avail - pad) : 0;
      pad   = seg_w > 0 ? pad : 0;
    }

    remaining -= pad + seg_w;
    drawn = drawn || seg_w > 0;
  }

  return cols;
}

std::pair<unsigned int, unsigned int> TextRow::size() {
  unsigned int w = 0, h = rows();

  for (const auto &[pad, seg_w] : layout()) {
    w += pad + seg_w;
  }

  if (w == 0 || h == 0) {
    return {0, 0};
  }

  return {w, h}; // returned as width, height
}

void TextRow::render() {
  const unsigned int h = rows();

  if (h == 0) {
    return;
  }

  // padding is an empty Text, which fills its block with spaces, so it's drawn just like a segment
  std::vector<Text> cols;

  for (int i = 0; const auto &[pad, seg_w] : layout()) {
    if (pad > 0) {
      cols.push_back(Text("", {}, pad, h));
    }
    if (seg_w > 0) { // a width of 0 draws nothing, so the cursor hasn't moved
      cols.push_back(Text(_segments[i].text).size(seg_w, h));
    }
    i++;
  }

  for (int i = 0; i < cols.size(); i++) {
    cols[i].render(); // ends on the bottom row, just right of the column

    if (i != cols.size() - 1) { // the final column leaves the cursor on the bottom row, like Text
      terminal.CursorUp(h - 1); // back to the top row for the next column
    }
  }
}

} // namespace termui

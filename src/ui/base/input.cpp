#include <termui/ui/ui.hpp>

#include <algorithm>
#include <deque>

namespace termui {

Input::Input(const termui::string &value, const termui::string &placeholder, const Style &valStyle, const Style &plhStyle)
  : _value(value), _placeholder(placeholder), _valStyle(valStyle), _plhStyle(plhStyle), _w(std::nullopt), _h(std::nullopt){};

Input &Input::value(const termui::string &v) {
  _value = v;
  return *this;
}

Input &Input::placeholder(const termui::string &p) {
  _placeholder = p;
  return *this;
}

Input &Input::valueStyle(const Style &s) {
  _valStyle = s;
  return *this;
}

Input &Input::placeholderStyle(const Style &s) {
  _plhStyle = s;
  return *this;
}

Input &Input::width(unsigned int width) {
  _w = width;
  return *this;
}

Input &Input::height(unsigned int height) {
  _h = height;
  return *this;
}

Input &Input::size(unsigned int width, unsigned int height) {
  _w = width;
  _h = height;
  return *this;
}

Input &Input::focus() {
  _focussed = true;
  return *this;
}

Input &Input::blur() {
  _focussed = false;
  return *this;
}

Input &Input::focussed(bool x) {
  _focussed = x;
  return *this;
}

const termui::string &Input::get_value() const { return _value; }

void Input::render() {
  bool has_w = _w.has_value(), has_h = _h.has_value();
  int  w = *_w, h = *_h;

  if ((has_w && w == 0) || (has_h && h == 0)) {
    return;
  }

  std::string &v = _value;
  std::string &p = _placeholder;

  bool use_response    = !v.empty();
  bool use_placeholder = !use_response && !p.empty();

  const std::string      &src = use_response ? v : p;
  std::deque<std::string> formatted;
  size_t                  pos(0), next(0); // byte offsets into src: start and end of the current line

  if (use_response && has_h && h == 1) {
    // Keep w - 1 chars so a cell stays free for the cursor: a line of exactly w chars never reaches
    // the (vizlen < w) branch below, leaving row and col unset for MoveCursor when value >= w wide.
    // w == 1 is handled here as reverse_max_visible_length(s, 0) returns all of s, not none of it,
    // which would write the whole value into a one cell input.
    const size_t tail = (w > 1) ? reverse_max_visible_length(src, w - 1) : 0;

    // sgr codes in the scrolled off part still style the kept text, so carry them over
    formatted.push_back(sgr_sequences(src, src.size() - tail) + std::string(src.end() - tail, src.end()));
  }
  else {
    constexpr size_t npos = std::string::npos;

    while (pos < src.size()) {
      bool is_last_line  = (src.find('\n', pos) == npos) && (has_h && formatted.size() == h - 1);
      // only placeholders truncate: stopping here on a value would drop the end of it, which is where
      // the user is typing. Values keep wrapping, and the trim below keeps the last h lines instead.
      bool will_truncate = use_placeholder && is_last_line && visible_length(src, pos) > w;

      if (will_truncate) {
        // Last line and it doesn't all fit: ignore word boundaries, take as much
        // as possible while leaving room for the ellipsis, and stop.
        // size_t budget = (w > 1) ? (w - 1) : 0;
        next = max_visible_length(src, w /*budget*/, pos);

        formatted.push_back(src.substr(pos, next - pos));
        break;
      }

      const size_t limit = has_w ? max_visible_length(src, w, pos) : npos; // byte index where this line runs out of width
      const size_t rsp   = has_w ? src.rfind(' ', limit) : npos;           // last space at or before limit, anywhere in src
      const size_t sp    = (rsp == npos || rsp < pos) ? npos : rsp;        // word-wrap point: rsp if on this line (earlier spaces belong to previous lines)
      const bool   fits  = !has_w || visible_length(src, pos) <= w;        // whether all remaining text fits on this line

      // clang-format off
      next = std::min({
        src.size(),
        limit,
        src.find('\n', pos),
        (sp == npos || fits) ? src.size() : sp
      });
      // clang-format on

      formatted.push_back(src.substr(pos, next - pos));

      if (next >= src.size()) {
        break;
      }

      pos = (src[next] == '\n' || src[next] == ' ') ? next + 1 : next;
    }
  }

  // moves cursor over to the next line, doesnt affect underlying data, just a visual effect.
  // compares visible chars, not bytes: a full line of utf-8 or sgr text has size() > w, and would
  // leave the cursor past the end of the line. The placeholder cursor stays at the start, so skip it.
  if (use_response && formatted.size() > 0 && visible_length(formatted.back()) == w && h > 1) {
    formatted.push_back("");
  }

  if (use_response && v.ends_with("\n")) {
    formatted.push_back("");
  }

  if (formatted.size() > h) { // trim if too many lines are in the vector
    std::string carried;      // sgr codes on the trimmed lines still style the lines we keep

    for (size_t i = 0; i < formatted.size() - h; i++) {
      carried += sgr_sequences(formatted.at(i));
    }

    formatted       = std::deque(formatted.end() - h, formatted.end());
    formatted.at(0) = carried + formatted.at(0);
  }

  int row, col; // used to keep track of which position to restore cursor to

  // default to the start of the input: covers the placeholder, and an input with nothing to
  // write, where neither loop below runs and MoveCursor would otherwise read these unset
  std::tie(row, col) = terminal.GetFutureCursorPosition();

  if (use_response) {
    terminal.StyleStack(_valStyle);
  }
  else if (use_placeholder) {
    terminal.StyleStack(_plhStyle);
  }

  if (use_response) {
    for (int i = 0; i < formatted.size(); i++) {
      auto vizlen = visible_length(formatted.at(i));
      if (i > 0) {
        terminal.CursorDown(1).CursorLeft(w);
      }

      terminal.write(formatted.at(i));

      if (vizlen <= w) {
        std::tie(row, col) = terminal.GetFutureCursorPosition();
      }

      if (vizlen < w) {
        terminal.write(std::string(w - vizlen, ' '));
      }
    }
  }

  if (use_placeholder) {
    for (int i = 0; i < formatted.size(); i++) {
      auto vizlen = visible_length(formatted.at(i));
      if (i > 0) {
        terminal.CursorDown(1).CursorLeft(w);
      }

      terminal.write(formatted.at(i));

      if (vizlen < w) {
        terminal.write(std::string(w - vizlen, ' '));
      }
    }
  }

  if (use_response || use_placeholder) { // nothing was pushed otherwise, and we'd pop the caller's style
    terminal.StylePop();
  }

  if (_focussed) {
    terminal.MoveCursor(row, col);
  }
}

} // namespace termui

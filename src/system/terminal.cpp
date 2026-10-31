#include <termui/system/terminal.hpp>

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

namespace termui {

Terminal &Terminal::instance() {
  static Terminal term;
  return term;
}

void Terminal::write(const std::string &str) const { _outbuff += str; } // writes to outbuff, not cout directly

void Terminal::write(unsigned int row, unsigned int column, const std::string &str) const {
  MoveCursor(row, column);
  _outbuff += str;
}

void Terminal::flush() const {
  std::cout << _outbuff << std::flush;
  _outbuff.clear();
}

std::string Terminal::read() const { // claude written
  static std::string buf;

  // only read from stdin if we don't already have a complete sequence buffered
  auto find_end = [&]() {
    if (buf.empty()) {
      return std::string::npos;
    }

    if (buf[0] == '\x1b' && buf.size() > 1) {
      return buf.find_first_of("Mm~ABCDFHPQRSZ", 2);
    }

    return std::string::npos; // single char, always "complete"
  };

  if (find_end() == std::string::npos) {
    char tmp[64];
    int  nbytes = ::read(STDIN_FILENO, tmp, sizeof(tmp));
    if (nbytes > 0) {
      buf.append(tmp, nbytes);
    }
  }

  if (buf.empty()) {
    return "";
  }

  // escape sequence
  if (buf[0] == '\x1b' && buf.size() > 1) {
    auto end = buf.find_first_of("Mm~ABCDFHPQRSZ", 2);

    if (end != std::string::npos) {
      std::string seq = buf.substr(0, end + 1);
      buf.erase(0, end + 1);
      return seq;
    }
    return ""; // incomplete, wait for more
  }

  // regular character
  std::string ch = buf.substr(0, 1);
  buf.erase(0, 1);
  return ch;
}

unsigned int Terminal::width() const {
  winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return w.ws_col;
}

unsigned int Terminal::height() const {
  winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return w.ws_row;
}

unsigned int Terminal::halfWidth() const { return width() / 2; }

unsigned int Terminal::halfHeight() const { return height() / 2; }

Color Terminal::ForegroundColor() const {
  termios old, raw;
  tcgetattr(0, &old);
  raw = old;
  raw.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(0, TCSANOW, &raw);

  ::write(1, "\033]10;?\033\\", 8);

  std::string r;
  pollfd      p{0, POLLIN, 0};
  char        c;
  while (poll(&p, 1, 100) > 0 && ::read(0, &c, 1) == 1) {
    r += c;
    if (c == '\a' || (c == '\\' && r.size() > 1 && r[r.size() - 2] == '\033')) {
      break;
    }
  }

  tcsetattr(0, TCSANOW, &old);

  auto pos = r.find("rgb");

  return Color(static_cast<uint8_t>(std::stoi(r.substr(pos + 4, 2), nullptr, 16)), static_cast<uint8_t>(std::stoi(r.substr(pos + 9, 2), nullptr, 16)),
               static_cast<uint8_t>(std::stoi(r.substr(pos + 14, 2), nullptr, 16)));
}

Color Terminal::BackgroundColor() const {
  termios old, raw;
  tcgetattr(0, &old);
  raw = old;
  raw.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(0, TCSANOW, &raw);

  ::write(1, "\033]11;?\033\\", 8);

  std::string r;
  pollfd      p{0, POLLIN, 0};
  char        c;
  while (poll(&p, 1, 100) > 0 && ::read(0, &c, 1) == 1) {
    r += c;
    if (c == '\a' || (c == '\\' && r.size() > 1 && r[r.size() - 2] == '\033')) {
      break;
    }
  }

  tcsetattr(0, TCSANOW, &old);

  auto pos = r.find("rgb");

  return Color(static_cast<uint8_t>(std::stoi(r.substr(pos + 4, 2), nullptr, 16)), static_cast<uint8_t>(std::stoi(r.substr(pos + 9, 2), nullptr, 16)),
               static_cast<uint8_t>(std::stoi(r.substr(pos + 14, 2), nullptr, 16)));
}

bool Terminal::HasDarkBackground() const {
  auto col = BackgroundColor();

  auto lum     = (0.299 * col.rgb.r) + (0.587 * col.rgb.g) + (0.114 * col.rgb.b);
  bool is_dark = lum < 127.5;

  return is_dark;
}

// ColorMode Terminal::ColorCapability();

//   void SetOutputMode(OutputMode om);

// ---- Styling ----
void Terminal::Style(termui::Style style) const {
  if (_color_capability == Terminal::Mode::Ascii) {
    return;
  }

  std::vector<int> modifier_codes;

  if (style.InheritsSGR()) {
    // Inherit: no action taken
  }
  else if (style.ResetsSGR()) {
    modifier_codes.push_back(0);
  }
  else {
    modifier_codes.push_back(0); // reset sgr before applying anew

    for (const auto &attr : style.attributes) {
      modifier_codes.push_back(static_cast<int>(attr));
    }
  }

  auto truecolor_to_ansi256 = [](uint8_t r, uint8_t g, uint8_t b) -> int {
    // map each component to 0..5
    auto conv = [](uint8_t c) -> int {
      if (c < 48)
        return 0;
      if (c < 115)
        return 1;
      return (int)((c - 35) / 40); // maps 0..255 to 0..5 roughly
    };
    int rr = conv(r), gg = conv(g), bb = conv(b);
    return 16 + 36 * rr + 6 * gg + bb;
  };

  // ansi256 code: ESC[38;5;{ID}m
  // trucol code:  ESC[38;2;{r};{g};{b}m

  if (style.foregroundColor.IsReset()) {
    modifier_codes.push_back(39);
  }
  else if (!style.foregroundColor.IsInherit()) {
    if (_color_capability == Terminal::Mode::Ansi256) {
      if (style.foregroundColor.mode == Color::Mode::Ansi256) {
        modifier_codes.push_back(38);
        modifier_codes.push_back(5);
        modifier_codes.push_back(style.foregroundColor.value);
      }
      else if (style.foregroundColor.mode == Color::Mode::TrueColor) {
        modifier_codes.push_back(38);
        modifier_codes.push_back(5);
        modifier_codes.push_back(truecolor_to_ansi256(style.foregroundColor.rgb.r, style.foregroundColor.rgb.g, style.foregroundColor.rgb.b));
      }
    }

    else if (_color_capability == Terminal::Mode::TrueColor) {
      if (style.foregroundColor.mode == Color::Mode::Ansi256) {
        modifier_codes.push_back(38);
        modifier_codes.push_back(5);
        modifier_codes.push_back(style.foregroundColor.value);
      }
      else if (style.foregroundColor.mode == Color::Mode::TrueColor) {
        modifier_codes.push_back(38);
        modifier_codes.push_back(2);
        modifier_codes.push_back(style.foregroundColor.rgb.r);
        modifier_codes.push_back(style.foregroundColor.rgb.g);
        modifier_codes.push_back(style.foregroundColor.rgb.b);
      }
    }
  }

  if (style.backgroundColor.IsReset()) {
    modifier_codes.push_back(49);
  }
  else if (!style.backgroundColor.IsInherit()) {
    if (_color_capability == Terminal::Mode::Ansi256) {
      if (style.backgroundColor.mode == Color::Mode::Ansi256) {
        modifier_codes.push_back(48);
        modifier_codes.push_back(5);
        modifier_codes.push_back(style.backgroundColor.value);
      }
      else if (style.backgroundColor.mode == Color::Mode::TrueColor) {
        modifier_codes.push_back(48);
        modifier_codes.push_back(5);
        modifier_codes.push_back(truecolor_to_ansi256(style.backgroundColor.rgb.r, style.backgroundColor.rgb.g, style.backgroundColor.rgb.b));
      }
    }

    else if (_color_capability == Terminal::Mode::TrueColor) {
      if (style.backgroundColor.mode == Color::Mode::Ansi256) {
        modifier_codes.push_back(48);
        modifier_codes.push_back(5);
        modifier_codes.push_back(style.backgroundColor.value);
      }
      else if (style.backgroundColor.mode == Color::Mode::TrueColor) {
        modifier_codes.push_back(48);
        modifier_codes.push_back(2);
        modifier_codes.push_back(style.backgroundColor.rgb.r);
        modifier_codes.push_back(style.backgroundColor.rgb.g);
        modifier_codes.push_back(style.backgroundColor.rgb.b);
      }
    }
  }

  if (modifier_codes.empty()) {
    return;
  }

  std::string attr_buff = "\x1b[";

  for (auto attr : modifier_codes) {
    attr_buff += std::to_string(static_cast<int>(attr)) + ";";
  }

  attr_buff = attr_buff.substr(0, attr_buff.size() - 1);

  attr_buff += "m";

  _outbuff += attr_buff;
}

void Terminal::StyleStack(termui::Style s) const {
  _style_stack.push(s);
  Style(_style_stack.top());
}

void Terminal::StylePop() const {
  if (_style_stack.size() > 0) {
    _style_stack.pop();
  }

  if (_style_stack.empty()) {
    Style(Styles::none);
  }
  else {
    Style(_style_stack.top());
  }
}

// --- Positioning ---
void Terminal::MoveCursor(unsigned int row, unsigned int column) const { _outbuff += std::format("\x1b[{};{}H", row, column); }

void Terminal::SaveCursorPosition() const { _outbuff += "\0337\033[s"; }

void Terminal::RestoreCursorPosition() const { _outbuff += "\0338\033[u"; }

std::tuple<int, int> Terminal::GetCursorPosition() const {
  termios oldt{}, newt{};
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  std::cout << "\033[6n" << std::flush; // query code

  std::string response; // response code: \033[{row};{col}R
  char        ch;
  while (::read(STDIN_FILENO, &ch, 1) == 1) {
    response += ch;
    if (ch == 'R')
      break;
  }

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

  int    row = -1, col = -1;
  size_t esc  = response.find('[');
  size_t semi = response.find(';');
  size_t r    = response.find('R');
  if (esc != std::string::npos && semi != std::string::npos && r != std::string::npos) {
    row = std::stoi(response.substr(esc + 1, semi - esc - 1));
    col = std::stoi(response.substr(semi + 1, r - semi - 1));
  }

  return {row, col};
}

std::tuple<int, int> Terminal::GetFutureCursorPosition() const {
  const int w = width();
  const int h = height();

  if (w <= 0 || h <= 0) {
    return {1, 1};
  }

  auto [row, col] = GetCursorPosition();

  row = std::clamp(row, 1, h);
  col = std::clamp(col, 1, w);

  const std::string &buf = _outbuff;
  const size_t       n   = buf.size();
  size_t             i   = 0;

  // Terminals commonly use delayed autowrap: writing the last
  // column sets wrapPending; the NEXT printable character wraps.
  bool wrapPending = false;

  // Saved cursor for ESC 7/8 and CSI s/u.
  int  savedRow         = row;
  int  savedCol         = col;
  bool savedWrapPending = wrapPending;

  // Approximate terminal newline mode (LNM).
  // Set this to match your actual terminal configuration.
  constexpr bool newlineMode = false;

  auto clearWrap = [&] { wrapPending = false; };

  auto lineFeed = [&] {
    clearWrap();
    row = std::min(h, row + 1);
    if (newlineMode) {
      col = 1;
    }
  };

  auto carriageReturn = [&] {
    col = 1;
    clearWrap();
  };

  auto putCell = [&](int cells) {
    if (cells <= 0) {
      return;
    }

    if (wrapPending) {
      col = 0;
      if (row < h - 1) {
        ++row;
      }
      wrapPending = false;
    }

    // A wide glyph may need to wrap before it is written.
    if (cells > 1 && col + cells > w) {
      col = 0;
      if (row < h - 1) {
        ++row;
      }
    }

    // A glyph occupying the final cell leaves the cursor
    // there with wrapping pending.
    if (cells >= w - col) {
      col         = w - 1;
      wrapPending = true;
    }
    else {
      col += cells;
    }
  };

  auto parseParams = [](const std::string &s) {
    std::vector<int> out;
    size_t           p = 0;

    // Ignore private parameter prefixes such as '?', '>', '!'.
    while (p < s.size() && s[p] >= 0x3C && s[p] <= 0x3F) {
      ++p;
    }

    while (p < s.size()) {
      int  value    = 0;
      bool hasDigit = false;

      while (p < s.size() && s[p] >= '0' && s[p] <= '9') {
        hasDigit = true;
        if (value < 1000000) {
          value = value * 10 + (s[p] - '0');
        }
        ++p;
      }

      out.push_back(hasDigit ? value : 0);

      if (p >= s.size() || s[p] != ';') {
        break;
      }

      ++p;
    }

    return out;
  };

  auto param = [](const std::vector<int> &p, size_t index, int defaultValue = 1) {
    if (index >= p.size() || p[index] == 0) {
      return defaultValue;
    }
    return p[index];
  };

  // Consume UTF-8 sequences. wcwidth() returns the number of
  // terminal columns for the decoded character.
  auto consumeUtf8 = [&](size_t &pos) -> int {
    const unsigned char first = buf[pos];

    wchar_t wc  = 0;
    size_t  len = 1;

    if (first < 0x80) {
      wc = first;
    }
    else {
      int      expected = 0;
      uint32_t cp       = 0;

      if ((first & 0xE0) == 0xC0) {
        expected = 2;
        cp       = first & 0x1F;
      }
      else if ((first & 0xF0) == 0xE0) {
        expected = 3;
        cp       = first & 0x0F;
      }
      else if ((first & 0xF8) == 0xF0) {
        expected = 4;
        cp       = first & 0x07;
      }
      else {
        ++pos;
        return 1; // Invalid UTF-8: replacement approximation.
      }

      if (pos + expected > n) {
        ++pos;
        return 1;
      }

      for (int k = 1; k < expected; ++k) {
        const unsigned char b = buf[pos + k];
        if ((b & 0xC0) != 0x80) {
          ++pos;
          return 1;
        }
        cp = (cp << 6) | (b & 0x3F);
      }

      // Reject overlong encodings, surrogates and invalid
      // Unicode code points.
      const uint32_t minimum = expected == 2 ? 0x80 : expected == 3 ? 0x800 : 0x10000;

      if (cp < minimum || cp > 0x10FFFF || (cp >= 0xD800 && cp <= 0xDFFF)) {
        ++pos;
        return 1;
      }

      wc  = static_cast<wchar_t>(cp);
      len = expected;
    }

    pos += len;

    int cells = ::wcwidth(wc);
    if (cells < 0) {
      cells = 1;
    }

    return cells;
  };

  while (i < n) {
    const unsigned char c = buf[i];

    // ESC sequences.
    if (c == 0x1B) {
      if (i + 1 >= n) {
        break;
      }

      const unsigned char next = buf[i + 1];

      // CSI: ESC [ parameters intermediates final
      if (next == '[') {
        i += 2;

        const size_t paramsStart = i;
        while (i < n && buf[i] >= 0x30 && buf[i] <= 0x3F) {
          ++i;
        }

        const std::string paramsText = buf.substr(paramsStart, i - paramsStart);

        while (i < n && buf[i] >= 0x20 && buf[i] <= 0x2F) {
          ++i;
        }

        if (i >= n) {
          break;
        }

        const unsigned char final = buf[i++];

        // Only cursor commands need numeric parameters.
        // Private CSI commands, such as ?25h, are ignored.
        const bool privateSequence = !paramsText.empty() && (paramsText[0] == '?' || paramsText[0] == '>' || paramsText[0] == '!' || paramsText[0] == '<');

        if (privateSequence) {
          continue;
        }

        const auto p = parseParams(paramsText);
        const int  a = param(p, 0);
        const int  b = param(p, 1);

        switch (final) {
        case 'A':
          row = std::max(1, row - a);
          clearWrap();
          break;
        case 'B':
          row = std::min(h, row + a);
          clearWrap();
          break;
        case 'C':
          col = std::min(w, col + a);
          clearWrap();
          break;
        case 'D':
          col = std::max(1, col - a);
          clearWrap();
          break;

        case 'E':
          row = std::min(h, row + a);
          col = 1;
          clearWrap();
          break;

        case 'F':
          row = std::max(1, row - a);
          col = 1;
          clearWrap();
          break;

        case 'G':
        case '`':
          col = std::clamp(a, 1, w);
          clearWrap();
          break;

        case 'd':
          row = std::clamp(a, 1, h);
          clearWrap();
          break;

        case 'H':
        case 'f':
          row = std::clamp(a, 1, h);
          col = std::clamp(b, 1, w);
          clearWrap();
          break;
        case 'a': // HPR: horizontal relative movement
          col = std::clamp(col + a, 0, w - 1);
          clearWrap();
          break;
        case 'e': // VPR: vertical relative movement
          row = std::clamp(row + a, 0, h - 1);
          clearWrap();
          break;
        case 's': // Save cursor (common CSI form)
          savedRow         = row;
          savedCol         = col;
          savedWrapPending = wrapPending;
          break;
        case 'u': // Restore cursor
          row         = savedRow;
          col         = savedCol;
          wrapPending = savedWrapPending;
          break;
        default:
          // Includes SGR (m), erase (J/K), and other
          // commands that don't directly move the cursor.
          break;
        }

        continue;
      }

      // OSC: ESC ] ... terminated by BEL or ST (ESC \).
      if (next == ']') {
        i += 2;
        while (i < n) {
          if (buf[i] == '\a') {
            ++i;
            break;
          }
          if (buf[i] == 0x1B && i + 1 < n && buf[i + 1] == '\\') {
            i += 2;
            break;
          }
          ++i;
        }
        continue;
      }

      // DCS, SOS, PM, APC: terminated by ST (ESC \).
      if (next == 'P' || next == 'X' || next == '^' || next == '_') {
        i += 2;
        while (i < n) {
          if (buf[i] == 0x1B && i + 1 < n && buf[i + 1] == '\\') {
            i += 2;
            break;
          }
          ++i;
        }
        continue;
      }

      // Common two-byte ESC commands.
      i += 2;
      switch (next) {
      case '7': // DECSC: save cursor
        savedRow         = row;
        savedCol         = col;
        savedWrapPending = wrapPending;
        break;
      case '8': // DECRC: restore cursor
        row         = savedRow;
        col         = savedCol;
        wrapPending = savedWrapPending;
        break;
      case 'D': // IND: index / line feed
        lineFeed();
        break;
      case 'E': // NEL: next line, column 0
        lineFeed();
        col = 0;
        break;
      case 'M': // RI: reverse index
        clearWrap();
        if (row > 0)
          --row;
        break;
      case 'c': // RIS: reset (approximate cursor effect)
        row = col = 0;
        clearWrap();
        break;
      default:
        // Ignore other ESC commands.
        break;
      }
      continue;
    }

    // C0 controls and DEL.
    switch (c) {
    case '\n':
      ++i;
      lineFeed();
      continue;
    case '\r':
      ++i;
      carriageReturn();
      continue;
    case '\b':
      ++i;
      col = std::max(0, col - 1);
      clearWrap();
      continue;
    case '\t':
      ++i;
      clearWrap();
      col = std::min(w - 1, ((col / 8) + 1) * 8);
      continue;
    case '\a': // BEL
      ++i;
      continue;
    case 0x7F: // DEL
      ++i;
      continue;
    default:
      break;
    }

    if (c < 0x20) {
      ++i; // Ignore other C0 controls.
      continue;
    }

    putCell(consumeUtf8(i));
  }

  return {row, col};
}

const Terminal &Terminal::CursorUp(unsigned int n) const {
  if (n > 0)
    _outbuff += std::format("\x1b[{}A", n);

  return *this;
}
const Terminal &Terminal::CursorDown(unsigned int n) const {
  if (n > 0)
    _outbuff += std::format("\x1b[{}B", n);

  return *this;
}
const Terminal &Terminal::CursorRight(unsigned int n) const {
  if (n > 0)
    _outbuff += std::format("\x1b[{}C", n);

  return *this;
}
const Terminal &Terminal::CursorLeft(unsigned int n) const {
  if (n > 0)
    _outbuff += std::format("\x1b[{}D", n);

  return *this;
}
//   void CursorNextLine(unsigned int n);
//   void CursorPrevLine(unsigned int n);

//   // --- Screen ---
//   void Reset();
//   void SaveScreen();
//   void RestoreScreen();
//   void AltScreen();
//   void ExitAltScreen();
void Terminal::ClearScreen() const { _outbuff += "\x1b[2J"; }
void Terminal::ClearScrollback() const { _outbuff += "\x1b[3J"; }
//   void ClearLine();
//   void ClearLines();
//   void InsertLines(unsigned int n);
//   void DeleteLines(unsigned int n);

//   void DisableInputBuffering();
//   void EnableInputBuffering();
//   void DisableInputEcho();
//   void EnableInputEcho();

//   // --- Session ---
//   void SetWindowTitle(std::string);
//   void SetForegroundColor(Color);
//   void SetBackgroundColor(Color);
//   void SetCursorColor(Color);
void Terminal::ShowCursor() const { _outbuff += "\x1b[?25h"; }
void Terminal::HideCursor() const { _outbuff += "\x1b[?25l"; }

//   // void Copy(msg?);
//   // void CopyPrimary(msg?);
//   void Notify(std::string title, std::string body);

//   // --- Mouse ---
//   void EnableMousePress(); // enables X10 mouse mode (button presses
//   void DisableMousePress();
//   void EnableMouseTracking();
//   void DisableMouseTracking();

Terminal::Terminal() : _color_capability(Terminal::Mode::Ascii) {
  // do color capability detection here

  const char *colorterm = std::getenv("COLORTERM");
  if (colorterm) {
    std::string val(colorterm);
    if (val == "truecolor" || val == "24bit") {
      _color_capability = Terminal::Mode::TrueColor;
    }
  }

  const char *term = std::getenv("TERM");
  if (term) {
    std::string val(term);
    if (val == "xterm-256color" && _color_capability != Terminal::Mode::TrueColor) {
      _color_capability = Terminal::Mode::Ansi256;
    }
  }
}

TermSetup::TermSetup()
  : input_buffering(true), input_echoing(true), show_cursor(true), alternate_output_buffer(false), enable_mouse_reporting(false){}; // terminal defaluts

TermSetup::TermSetup(bool input_buffering, bool input_echoing, bool show_cursor, bool alternate_output_buffer, bool enable_mouse_reporting)
  : input_buffering(input_buffering),
    input_echoing(input_echoing),
    show_cursor(show_cursor),
    alternate_output_buffer(alternate_output_buffer),
    enable_mouse_reporting(enable_mouse_reporting){};

void TermSetup::configure() {
  // assume terminal may not be in default settings
  // and bypass Terminal::outbuff -> straight to cout

  termios t;

  auto io_buff_off = [&]() -> void {
    tcgetattr(STDIN_FILENO, &t);          // get the current terminal i/o flags
    t.c_lflag &= ~ICANON;                 // flip the bit related to buffering
    tcsetattr(STDIN_FILENO, TCSANOW, &t); // apply new settings
  };
  auto io_buff_on = [&]() -> void {
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
  };

  auto echo_off = [&]() -> void {
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
  };
  auto echo_on = [&]() -> void {
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
  };

  (input_buffering == true) ? io_buff_on() : io_buff_off();
  (input_echoing == true) ? echo_on() : echo_off();

  // clang-format off
  std::cout << ((show_cursor == true) ? "\x1b[?25h" : "\x1b[?25l") 
            << ((alternate_output_buffer == true) ? "\x1b[?1049h" : "\x1b[?1049l")
            << ((enable_mouse_reporting == true) ? "\x1b[?1003h\x1b[?1006h" : "\x1b[?1003l\x1b[?1006l") 
            << std::flush;
  // clang-format on
}

void TermSetup::reset() { // sets terminal defaults, not blindly inversing configure
  termios t;

  auto io_buff_on = [&]() -> void {
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
  };

  auto echo_on = [&]() -> void {
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
  };

  io_buff_on();
  echo_on();

  std::cout << "\x1b[?25h"              // show cursor
            << "\x1b[?1049l"            // primary output buffer
            << "\x1b[?1003l\x1b[?1006l" // mouse reporting off
            << std::flush;
}

} // namespace termui

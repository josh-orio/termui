#include <termui/system/terminal.hpp>

namespace termui {

Terminal &Terminal::instance() {
  static Terminal term;
  return term;
}

void Terminal::write(const std::string &str) const { _outbuff += str; } // writes to outbuff, not cout directly

void Terminal::write(uint row, uint column, const std::string &str) const {
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
    if (buf.empty())
      return std::string::npos;
    if (buf[0] == '\x1b' && buf.size() > 1)
      return buf.find_first_of("Mm~ABCDFHPQRS", 2);
    return std::string::npos; // single char, always "complete"
  };

  if (find_end() == std::string::npos) {
    char tmp[64];
    int  nbytes = ::read(STDIN_FILENO, tmp, sizeof(tmp));
    if (nbytes > 0)
      buf.append(tmp, nbytes);
  }

  if (buf.empty())
    return "";

  // escape sequence
  if (buf[0] == '\x1b' && buf.size() > 1) {
    auto end = buf.find_first_of("Mm~ABCDFHPQRS", 2);
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

uint Terminal::width() const {
  winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return w.ws_col;
}

uint Terminal::height() const {
  winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return w.ws_row;
}

uint Terminal::halfWidth() const { return width() / 2; }

uint Terminal::halfHeight() const { return height() / 2; }

//   Color ForegroundColor(){}
//   Color BackgroundColor(){}
//   bool HasDarkBackground(){}
//   ColorMode ColorCapability();

//   void SetOutputMode(OutputMode om);

// ---- Styling ----
void Terminal::Style(termui::Style style) const {
  if (_color_capability == Terminal::Mode::ASCII) {
    return;
  }

  std::vector<int> modifier_codes;

  if (style.InheritsSGR()) {
    // Inherit: no action taken

  } else if (style.ResetsSGR()) {
    modifier_codes.push_back(0);

  } else {
    modifier_codes.push_back(0); // reset sgr before applying anew

    for (const auto &attr : style.Attributes()) {
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

  if (style.Foreground().IsReset()) {
    modifier_codes.push_back(39);

  } else if (!style.Foreground().IsInherit()) {
    if (_color_capability == Terminal::Mode::ANSI256) {
      if (style.Foreground().mode == Color::Mode::ANSI256) {
        modifier_codes.push_back(38);
        modifier_codes.push_back(5);
        modifier_codes.push_back(style.Foreground().value);

      } else if (style.Foreground().mode == Color::Mode::TRUECOLOR) {
        modifier_codes.push_back(38);
        modifier_codes.push_back(5);
        modifier_codes.push_back(truecolor_to_ansi256(style.Foreground().rgb.r, style.Foreground().rgb.g, style.Foreground().rgb.b));
      }
    }

    else if (_color_capability == Terminal::Mode::TRUECOLOR) {
      if (style.Foreground().mode == Color::Mode::ANSI256) {
        modifier_codes.push_back(38);
        modifier_codes.push_back(5);
        modifier_codes.push_back(style.Foreground().value);

      } else if (style.Foreground().mode == Color::Mode::TRUECOLOR) {
        modifier_codes.push_back(38);
        modifier_codes.push_back(2);
        modifier_codes.push_back(style.Foreground().rgb.r);
        modifier_codes.push_back(style.Foreground().rgb.g);
        modifier_codes.push_back(style.Foreground().rgb.b);
      }
    }
  }

  if (style.Background().IsReset()) {
    modifier_codes.push_back(49);

  } else if (!style.Background().IsInherit()) {
    if (_color_capability == Terminal::Mode::ANSI256) {
      if (style.Background().mode == Color::Mode::ANSI256) {
        modifier_codes.push_back(48);
        modifier_codes.push_back(5);
        modifier_codes.push_back(style.Background().value);

      } else if (style.Background().mode == Color::Mode::TRUECOLOR) {
        modifier_codes.push_back(48);
        modifier_codes.push_back(5);
        modifier_codes.push_back(truecolor_to_ansi256(style.Background().rgb.r, style.Background().rgb.g, style.Background().rgb.b));
      }
    }

    else if (_color_capability == Terminal::Mode::TRUECOLOR) {
      if (style.Background().mode == Color::Mode::ANSI256) {
        modifier_codes.push_back(48);
        modifier_codes.push_back(5);
        modifier_codes.push_back(style.Background().value);

      } else if (style.Background().mode == Color::Mode::TRUECOLOR) {
        modifier_codes.push_back(48);
        modifier_codes.push_back(2);
        modifier_codes.push_back(style.Background().rgb.r);
        modifier_codes.push_back(style.Background().rgb.g);
        modifier_codes.push_back(style.Background().rgb.b);
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

  } else {
    Style(_style_stack.top());
  }
}

// --- Positioning ---
void Terminal::MoveCursor(uint row, uint column) const { _outbuff += std::format("\x1b[{};{}H", row, column); }

void Terminal::SaveCursorPosition() const { _outbuff += "\033[s"; }

void Terminal::RestoreCursorPosition() const { _outbuff += "\033[u"; }

const Terminal &Terminal::CursorUp(uint n) const {
  if (n > 0)
    _outbuff += std::format("\x1b[{}A", n);

  return *this;
}
const Terminal &Terminal::CursorDown(uint n) const {
  if (n > 0)
    _outbuff += std::format("\x1b[{}B", n);

  return *this;
}
const Terminal &Terminal::CursorRight(uint n) const {
  if (n > 0)
    _outbuff += std::format("\x1b[{}C", n);

  return *this;
}
const Terminal &Terminal::CursorLeft(uint n) const {
  if (n > 0)
    _outbuff += std::format("\x1b[{}D", n);

  return *this;
}
//   void CursorNextLine(uint n);
//   void CursorPrevLine(uint n);

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
//   void InsertLines(uint n);
//   void DeleteLines(uint n);

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

Terminal::Terminal() : _color_capability(Terminal::Mode::ASCII) {
  // do color capability detection here

  const char *colorterm = std::getenv("COLORTERM");
  if (colorterm) {
    std::string val(colorterm);
    if (val == "truecolor" || val == "24bit") {
      _color_capability = Terminal::Mode::TRUECOLOR;
    }
  }

  const char *term = std::getenv("TERM");
  if (term) {
    std::string val(term);
    if (val == "xterm-256color" && _color_capability != Terminal::Mode::TRUECOLOR) {
      _color_capability = Terminal::Mode::ANSI256;
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

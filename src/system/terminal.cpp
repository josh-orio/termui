#include <termui/system/terminal.hpp>

namespace termui {

Terminal &Terminal::instance() {
  static Terminal term;
  return term;
}

const Renderer &Terminal::GetRenderer() const { return renderer; }

void Terminal::write(const std::string &str) const { outbuff += str; } // writes to outbuff, not cout directly

void Terminal::write(uint row, uint column, const std::string &str) const {
  MoveCursor(row, column);
  outbuff += str;
}

void Terminal::flush() const {
  std::cout << outbuff << std::flush;
  outbuff.clear();
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

//   Color ForegroundColor(){}
//   Color BackgroundColor(){}
//   bool HasDarkBackground(){}
//   ColorMode ColorCapability();

//   void SetOutputMode(OutputMode om);

// --- Positioning ---
void Terminal::MoveCursor(uint row, uint column) const { outbuff += std::format("\x1b[{};{}H", row, column); }
//   void SaveCursorPosition();
//   void RestoreCursorPosition();
void Terminal::CursorUp(uint n) const {
  if (n == 0)
    return;

  outbuff += std::format("\x1b[{}A", n);
}
void Terminal::CursorDown(uint n) const {
  if (n == 0)
    return;

  outbuff += std::format("\x1b[{}B", n);
}
void Terminal::CursorForward(uint n) const {
  if (n == 0)
    return;

  outbuff += std::format("\x1b[{}C", n);
}
void Terminal::CursorBack(uint n) const {
  if (n == 0)
    return;

  outbuff += std::format("\x1b[{}D", n);
}
//   void CursorNextLine(uint n);
//   void CursorPrevLine(uint n);

//   // --- Screen ---
//   void Reset();
//   void SaveScreen();
//   void RestoreScreen();
//   void AltScreen();
//   void ExitAltScreen();
void Terminal::ClearScreen() const { outbuff += "\x1b[2J"; }
void Terminal::ClearScrollback() const { outbuff += "\x1b[3J"; }
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
void Terminal::ShowCursor() const { outbuff += "\x1b[?25h"; }
void Terminal::HideCursor() const { outbuff += "\x1b[?25l"; }

//   // void Copy(msg?);
//   // void CopyPrimary(msg?);
//   void Notify(std::string title, std::string body);

//   // --- Mouse ---
//   void EnableMousePress(); // enables X10 mouse mode (button presses
//   void DisableMousePress();
//   void EnableMouseTracking();
//   void DisableMouseTracking();

Terminal::Terminal() : renderer(ColorMode::ASCII) {
  // do color capability detection here

  const char *colorterm = std::getenv("COLORTERM");
  if (colorterm) {
    std::string val(colorterm);
    if (val == "truecolor" || val == "24bit") {
      color_capability = ColorMode::TRUECOLOR;
      renderer = Renderer(color_capability);
    }
  }

  const char *term = std::getenv("TERM");
  if (term) {
    std::string val(term);
    if (val == "xterm-256color" && color_capability != ColorMode::TRUECOLOR) {
      color_capability = ColorMode::ANSI256;
      renderer = Renderer(color_capability);
    }
  }
}

TermSetup::TermSetup()
  : input_buffering(true), input_echoing(true), show_cursor(true), alternate_output_buffer(false), enable_mouse_reporting(false) {}; // terminal defaluts

TermSetup::TermSetup(bool input_buffering, bool input_echoing, bool show_cursor, bool alternate_output_buffer, bool enable_mouse_reporting)
  : input_buffering(input_buffering),
    input_echoing(input_echoing),
    show_cursor(show_cursor),
    alternate_output_buffer(alternate_output_buffer),
    enable_mouse_reporting(enable_mouse_reporting) {};

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

  std::cout << "\x1b[?25h"            // show cursor
            << "\x1b[?1049l"          // primary output buffer
            << "\x1b[?1003l\x1b[?1006l" // mouse reporting off
            << std::flush;
}

} // namespace termui

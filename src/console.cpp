#include <termui/console.hpp>

namespace termui {

void BufferModeToggle::off() {
  tcgetattr(STDIN_FILENO, &t);          // get the current terminal i/o flags
  t.c_lflag &= ~ICANON;                 // flip the bit related to buffering
  tcsetattr(STDIN_FILENO, TCSANOW, &t); // apply new settings
}

void BufferModeToggle::on() {
  // same as off() but inversed
  tcgetattr(STDIN_FILENO, &t);
  t.c_lflag |= ICANON;
  tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void EchoModeToggle::off() {
  tcgetattr(STDIN_FILENO, &t);
  t.c_lflag &= ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void EchoModeToggle::on() {
  tcgetattr(STDIN_FILENO, &t);
  t.c_lflag |= ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void CursorModeToggle::off() { std::cout << term::HIDE_CURSOR << std::flush; }
void CursorModeToggle::on() { std::cout << term::SHOW_CURSOR << std::flush; }

void AlternateBufferToggle::enable() { std::cout << term::ALT_BUFFER << std::flush; }
void AlternateBufferToggle::disable() { std::cout << term::PRIMARY_BUFFER << std::flush; }

void MouseReportingToggle::enable() { std::cout << term::ENABLE_MOUSE_REPORTING << std::flush; }
void MouseReportingToggle::disable() { std::cout << term::DISABLE_MOUSE_REPORTING << std::flush; }

MouseInteraction::MouseInteraction(std::string s) : valid(true) {
  int button, x, y;
  char mstate;

  if (sscanf((char *)s.data(), "\e[<%d;%d;%d%c", &button, &x, &y, &mstate) == 4) {
    col = x;
    row = y;

    if (mstate == 'm') {
      event = MouseEventType::BUTTON_RELEASE;

    } else {
      if (button == 64) {
        event = MouseEventType::SCROLL_UP;

      } else if (button == 65) {
        event = MouseEventType::SCROLL_DOWN;

      } else if (button >= 32 && button <= 35) {
        event = MouseEventType::MOVE;

      } else if (button == 0) {
        event = MouseEventType::LEFT_CLICK;

      } else if (button == 1) {
        event = MouseEventType::MIDDLE_CLICK;

      } else if (button == 2) {
        event = MouseEventType::RIGHT_CLICK;
      }
    }
  } else {
    valid = false; // signal that SGR/input code is not valid
  }
}

bool MouseInteraction::match(MouseEventType met) { return (met == event); }

bool MouseInteraction::match(MouseEventType met, size_t x1, size_t x2, size_t y1, size_t y2) {
  bool event_match = (met == event);
  bool within_vertical = (y1 <= row) && (row <= y2);
  bool within_horiz = (x1 <= col) && (col <= x2);

  return event_match && within_vertical && within_horiz;
}

Console::Console(bool b, bool e, bool c, bool a, bool m) : outbuff() {
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  width = w.ws_col, height = w.ws_row;

  buffered = b;
  echos = e;
  cursor = c;
  altterm = a;
  mouserep = m;
}

void Console::show() {
  (buffered == true) ? bt.on() : bt.off();
  (echos == true) ? et.on() : et.off();
  (cursor == true) ? ct.on() : ct.off();
  (altterm == true) ? at.enable() : at.disable();
  (mouserep == true) ? mt.enable() : mt.disable();
}

void Console::close() {
  // switch t for f should have the right effect?
  (buffered == false) ? bt.on() : bt.off();
  (echos == false) ? et.on() : et.off();
  (cursor == false) ? ct.on() : ct.off();
  (altterm == false) ? at.enable() : at.disable();
  (mouserep == false) ? mt.enable() : mt.disable();
}

void Console::clear_outbuff() { outbuff.clear(); }

void Console::clear_screen() { std::cout << term::CLEAR_CONSOLE << term::CLEAR_SCROLLBACK; }

void Console::clear_scrollback() { std::cout << term::CLEAR_SCROLLBACK; }

void Console::print(int row, int col, std::string s) { outbuff += std::format("\e[{};{}H{}", row, col, s); }

void Console::print(std::string s) { outbuff += s; }

void Console::flush(bool s_clr, bool sb_clr) {
  if (sb_clr) {
    clear_scrollback();
  }
  if (s_clr) {
    clear_screen();
  }

  std::cout << outbuff << std::flush; // might not (wont) print without std::flush (due to read() in await_input i think?)
  clear_outbuff();
}

std::string Console::poll_input() {
  char keys[16]; // allow reading upto 16 chars at once, max expected is SGR mouse reporting codes
  int nbytes;

  nbytes = read(STDIN_FILENO, keys, sizeof(keys));

  return std::string(keys, nbytes);
}

void Console::update_size() {
  width = w.ws_col;
  height = w.ws_row;
}

} // namespace termui

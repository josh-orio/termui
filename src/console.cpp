#include "console.hpp"

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

void CursorModeToggle::off() { std::cout << term::HIDE_CURSOR; }
void CursorModeToggle::on() { std::cout << term::SHOW_CURSOR; }

void AlternateBufferToggle::enable() { std::cout << term::ALT_BUFFER; }
void AlternateBufferToggle::disable() { std::cout << term::PRIMARY_BUFFER; }

Console::Console() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  width = w.ws_col, height = w.ws_row;

  buffered = false;
  echos = false;
  cursor = false;
  altterm = true;

  outbuff.clear();
}

Console::Console(bool b, bool e, bool c, bool a) {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  width = w.ws_col, height = w.ws_row;

  buffered = b;
  echos = e;
  cursor = c;
  altterm = a;

  outbuff = "";
}

void Console::show() {
  (buffered == true) ? bt.on() : bt.off();
  (echos == true) ? et.on() : et.off();
  (cursor == true) ? ct.on() : ct.off();
  (altterm == true) ? at.enable() : at.disable();
}

void Console::close() {
  // switch t for f should have the right effect?
  (buffered == false) ? bt.on() : bt.off();
  (echos == false) ? et.on() : et.off();
  (cursor == false) ? ct.on() : ct.off();
  (altterm == false) ? at.enable() : at.disable();
}

void Console::clear_outbuff() { outbuff.clear(); }

void Console::clear_screen() { std::cout << term::CLEAR_CONSOLE << term::CLEAR_SCROLLBACK; }

void Console::clear_scrollback() { std::cout << term::CLEAR_SCROLLBACK; }

void Console::print(int row, int col, std::string s) { outbuff += std::format("\e[{};{}H{}", row, col, s); }
void Console::print(std::string s) { outbuff += s; }

void Console::flush() {
  clear_scrollback();
  clear_screen();
  std::cout << outbuff << std::flush; // might not (wont) print without std::flush (due to read() in await_input i think?)
  clear_outbuff();
}

void Console::curs_up(int n) { outbuff += std::format("\e[{}A", n); }
void Console::curs_down(int n) { outbuff += std::format("\e[{}B", n); }
void Console::curs_right(int n) { outbuff += std::format("\e[{}C", n); }
void Console::curs_left(int n) { outbuff += std::format("\e[{}D", n); }

std::string Console::poll_input() {
  char keys[8]; // allow reading upto 8 chars at once, ansi codes should only be 3-5
  int nbytes;

  nbytes = read(STDIN_FILENO, keys, sizeof(keys));

  return std::string(keys, nbytes);
}

void Console::update_size() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  width = w.ws_col, height = w.ws_row;
}

} // namespace termui

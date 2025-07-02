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

void AlternateTerminalToggle::enable() { std::cout << term::ALT_BUFFER; }

void AlternateTerminalToggle::disable() { std::cout << term::PRIMARY_BUFFER; }

Console::Console(){};

Console::Console(bool b, bool e, bool c, bool a) {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  width = w.ws_col, height = w.ws_row;

  buffered = b;
  echos = e;
  cursor = c;
  altterm = a;
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

void Console::clear_buffer() { buffer = {}; }

void Console::clear_screen() { std::cout << term::CLEAR_CONSOLE << term::CLEAR_SCROLLBACK; }

void Console::clear_scrollback() { std::cout << term::CLEAR_SCROLLBACK; }

void Console::print(int row, int col, std::string s) { buffer.push_back(std::format("\033[{};{}H{}", row, col, s)); }

void Console::flush() {
  std::string stream = "";

  for (auto s : buffer) {
    stream += s;
  }

  clear_screen();
  std::cout << stream;
  clear_buffer();
}

void Console::update_size() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  width = w.ws_col, height = w.ws_row;
}

} // namespace termui

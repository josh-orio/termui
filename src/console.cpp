#include "console.hpp"

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

void CursorModeToggle::off() {
  std::cout << "\e[?25l"; // hides the cursor
}

void CursorModeToggle::on() {
  std::cout << "\e[?25h"; // enables the cursor
}

void AlternateTerminalToggle::enable() {
  std::cout << "\033[?1049h"; // switch to alternate terminal buffer
}

void AlternateTerminalToggle::disable() {
  std::cout << "\033[?1049l"; // switch back to the primary screen buffer
}

Console::Console() {};

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

void Console::print(std::string p) { std::cout << p << " "; }

void Console::print_ln() { std::cout << std::endl; }

void Console::print_ln(std::string p) { std::cout << p << std::endl; }

void Console::exact_print(std::string p) { std::cout << p; }

void Console::blank_ln() { std::cout << std::endl; }

void Console::print_at_pos(std::string p, int col, int row) {
  std::cout << std::format("\033[{};{}H{}", col, row, p);
};

void Console::print_ln_at_pos(std::string p, int col, int row) {
  std::cout << std::format("\033[{};{}H{}\n", col, row, p);
};

void Console::clear() {
  std::cout << "\033[2J"; // clear screen
  std::cout << "\033[3J"; // clear the scrollback buffer
  std::cout << "\033[H";  // move the cursor to the top-left
}

void Console::clear_scrollback() { std::cout << "\033[3J"; }

void Console::update_size() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  width = w.ws_col, height = w.ws_row;
}

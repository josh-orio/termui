#include <termui/termui.hpp>

int main() {
  // std::cout << "abcdabcdab";

  termui::terminal.HideCursor();
  termui::terminal.MoveCursor(10, 15);
  termui::terminal.flush();

  auto [row, col] = termui::terminal.GetCursorPosition();

  std::cout << row << " " << col << std::endl;

  return 0;
}

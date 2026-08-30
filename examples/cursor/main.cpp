#include <termui/termui.hpp>

int main() {
  std::cout << "abcdabcdab";

  auto [row, col] = termui::terminal.GetCursorPosition();

  std::cout << row << " " << col << std::endl;

  return 0;
}

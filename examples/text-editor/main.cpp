#include <termui/termui.hpp>

int main() {
  termui::TextEditor te("Text Editor Text Editor", termui::string());

  te.show();

  return 0;
}

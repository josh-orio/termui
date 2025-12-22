#include <termui/termui.hpp>

int main() {
  termui::string title("Info Page Demo"), text("this\nis\na\nbunch\nof\ninfo\nthat\nneeds\nto\nbe\ndisplayed.");

  termui::InfoBox(title, text).show();

  return 0;
}

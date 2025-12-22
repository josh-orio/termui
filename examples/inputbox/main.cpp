#include <termui/termui.hpp>

int main() {
  termui::string title("Some input field"), response, placeholder("Start typing...");

  termui::InputBox(title, response, placeholder).show();

  return 0;
}

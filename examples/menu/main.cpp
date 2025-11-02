#include <termui.hpp>

int main() {
  termui::strings options({"Pizza", "Burger", "Tendies"});

  termui::Menu m("TV Shows", options);

  m.show();

  return 0;
}

#include <termui.hpp>

int main() {
  termui::strings shows({"Family Guy", "The Simpsons"}), descs({"9.5/10", "6/10"});

  termui::FancyMenu fm("TV Shows", shows, descs);

  fm.show();

  return 0;
}

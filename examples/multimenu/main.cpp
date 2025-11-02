#include <termui.hpp>

int main() {
  termui::strings optional_extras({"Onboard WIFI", "Blanket", "Seat Pillow", "Slippers", "Hand Towels"});

  termui::MultiMenu m("Customize your experience", optional_extras);

  m.show();

  return 0;
}

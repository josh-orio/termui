#include <termui/termui.hpp>

int main() {
  termui::strings optional_extras({"Onboard WIFI", "Blanket", "Seat Pillow", "Slippers", "Hand Towels"});

  termui::ToggleMenu tm("Customize your experience", optional_extras);

  tm.show();

  return 0;
}

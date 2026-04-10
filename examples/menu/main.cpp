#include <termui/termui.hpp>

int main() {
  termui::strings options({"Pizza", "Burger", "Tendies", "Macaroni Cheese", "Chicken Wings", "Hot Honey Chicken Wings", "Currywurst Platter",
                           "Rack of Spare Ribs", "Loaded Fries", "Loaded Fries (Halal)", "Kebab"});

  termui::Menu m("TV Shows", options);

  m.show();

  return 0;
}

#include <termui/termui.hpp>

int main() {
  termui::string title("Some input fields");
  termui::strings fields ({"Entree", "Appetizer", "Main Course 1", "Main Course 2", "Dessert", "Beverages"}), responses (6, "");

  termui::InputPage(title, fields, responses).show();

  return 0;
}

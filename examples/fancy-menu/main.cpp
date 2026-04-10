#include <termui/termui.hpp>

int main() {
  std::vector<termui::FancyList::Element> rows = {{termui::string("Family Guy"), termui::string("9.5/10")},
                                                  {termui::string("The Simpsons"), termui::string("6/10")}};

  std::vector<termui::FancyList::Element> rows2 = {{"Family Guy", "9.5/10"}, {"The Simpsons", "6/10"}};

  termui::FancyMenu fm("TV Shows", rows2);

  fm.show();

  return 0;
}

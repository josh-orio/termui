#ifndef MENU_HPP
#define MENU_HPP

#include <termui/ui/base/padded-text.hpp>
#include <termui/ui/interfaces/interface.hpp>
#include <termui/ui/widgets/list.hpp>

namespace termui {

class Menu : public Interface {
public:
  Menu(const termui::string &title, const termui::strings &elements, uint line_seperation = 1);

  int   show();   // -1 = exit, >=0 = element selected at index
  uint  cursor(); // returns cursor
  State status(); // when the interface closes, it will either be EXIT or SELECT

private:
  termui::string  title;
  termui::strings elements;

  PaddedText      title_banner;
  List            list;
  static Text     control_banner;
  static StyleMap styles;

  uint       line_seperation;
  const uint lvo = 5; // list vertical overhead
  const uint lho = 4; // list horizontal overhead
  bool       reprint; // flag indicates if reprint is required

  void display();
  void process_input();
  void update_size();
};

} // namespace termui

#endif

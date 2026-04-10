#ifndef FANCYMENU_HPP
#define FANCYMENU_HPP

#include <termui/ui/base/padded-text.hpp>
#include <termui/ui/display/box.hpp>
#include <termui/ui/interfaces/interface.hpp>
#include <termui/ui/widgets/fancy-list.hpp>

namespace termui {

class FancyMenu : public Interface {
public:
  FancyMenu(const termui::string &title, const std::vector<FancyList::Element> &rows, uint line_seperation = 1);

  termui::Interface::State show();   // returns EXIT or SELECT on close
  uint                     cursor(); // returns cursor position

private:
  termui::string  title;
  termui::strings text, desc;

  PaddedText      title_banner;
  FancyList       list;
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

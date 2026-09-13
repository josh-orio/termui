#ifndef FANCYMENU_HPP
#define FANCYMENU_HPP

#include <termui/ui/base/padded-text.hpp>
#include <termui/ui/display/box.hpp>
#include <termui/ui/interfaces/interface.hpp>
#include <termui/ui/widgets/fancy-list.hpp>

namespace termui {

class FancyMenu : public Interface {
public:
  FancyMenu(const termui::string &title, const std::vector<FancyList::Element> &rows, unsigned int line_seperation = 1);

  termui::Interface::State show();   // returns EXIT or SELECT on close
  unsigned int             cursor(); // returns cursor position

private:
  termui::string  title;
  termui::strings text, desc;

  PaddedText      title_banner;
  FancyList       list;
  static Text     control_banner;
  static StyleMap styles;

  unsigned int       line_seperation;
  const unsigned int lvo = 5; // list vertical overhead
  const unsigned int lho = 4; // list horizontal overhead
  bool               reprint; // flag indicates if reprint is required

  void display();
  void process_input();
  void update_size();
};

} // namespace termui

#endif

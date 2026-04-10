#ifndef INFOBOX_HPP
#define INFOBOX_HPP

#include <termui/ui/base/padded-text.hpp>
#include <termui/ui/display/box.hpp>
#include <termui/ui/interfaces/interface.hpp>
#include <termui/ui/widgets/button.hpp>

namespace termui {

class InfoBox : public Interface {
public:
  InfoBox(const termui::string &t, const termui::string &c);

  void show();

private:
  termui::string title, content;

  Box                     box;
  Text                    header;
  Text                    body;
  Button                  close;
  static termui::Text     control_banner;
  static termui::StyleMap styles;

  uint w, h;
  bool reprint;

  void display();
  void process_input();
  void update_size();
};

} // namespace termui

#endif

#ifndef INFOPAGE_HPP
#define INFOPAGE_HPP

#include <termui/ui/base/padded-text.hpp>
#include <termui/ui/base/pager.hpp>
#include <termui/ui/interfaces/interface.hpp>

namespace termui {

class InfoPage : public Interface {
public:
  InfoPage(const termui::string &t, const termui::string &c);

  void show();

private:
  termui::string title, content;

  PaddedText      title_banner;
  Pager           info;
  static Text     control_strip;
  static StyleMap styles;

  const uint voh = 5; // vertical overhead (3 header + 2 footer)
  const uint hoh = 4; // horizontal overhead (2 left + 2 right)
  bool       reprint; // flag indicates if reprint is required

  void display();
  void process_input();
  void update_size();

  void cursor_up(uint count = 1);
  void cursor_down(uint count = 1);
};

} // namespace termui

#endif

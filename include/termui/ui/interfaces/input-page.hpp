#ifndef INPUTPAGE_HPP
#define INPUTPAGE_HPP

#include <termui/ui/base/padded-text.hpp>
#include <termui/ui/interfaces/interface.hpp>

namespace termui {

class InputPage : public Interface {
public:
  InputPage(const termui::string &title, const termui::strings &fields, const termui::strings &responses, unsigned int ls = 0);

  void show();

private:
  termui::string  title;
  termui::strings fields, responses;

  Text            header;
  static Text     control_strip;
  static StyleMap styles;

  unsigned int       cursor;          // index of selected row
  bool       selected;        // is row selected/active
  unsigned int       visible_lines;   // total rows in view
  unsigned int       start_line;      // index of first visible row
  unsigned int       line_seperation; // blank rows between elements
  const unsigned int voh = 5;         // vertical overhead (3 header + 2 footer)
  const unsigned int hoh = 4;         // horizontal overhead (2 left + 2 right)
  bool       reprint;

  void display();
  void process_input();
  void update_size();
};

} // namespace termui

#endif

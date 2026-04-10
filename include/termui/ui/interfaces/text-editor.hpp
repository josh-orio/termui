#ifndef TEXTEDITOR_HPP
#define TEXTEDITOR_HPP

#include <termui/ui/base/padded-text.hpp>
#include <termui/ui/base/pager.hpp>
#include <termui/ui/interfaces/interface.hpp>

namespace termui {

class TextEditor : public Interface {
public:
  TextEditor(const termui::string &title, const termui::string &content);

  void show();

private:
  termui::string title, content;

  PaddedText      title_banner;
  Pager           pager;
  static Text     control_strip;
  static StyleMap styles;

  const int toh = 2; // title overhead (1 col each side minimum)
  const int voh = 5; // vertical overhead (3 header + 2 footer)
  const int hoh = 5; // horizontal overhead (5 line numbers + 1 right)
  bool      reprint; // flag indicates if reprint is required

  void display();
  void process_input();
  void update_size();
};

} // namespace termui

#endif

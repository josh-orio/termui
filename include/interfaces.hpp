#ifndef INTERFACES_HPP
#define INTERFACES_HPP

#include "console.hpp"
#include "elements.hpp"
#include "unicode.hpp"
#include "util.hpp"
#include <memory>
#include <vector>

namespace termui {

class InfoBox {
public:
  termui::string title, content;
  int w, h;

  InfoBox(const termui::string &t, const termui::string &c);
  InfoBox(const std::string &t, const std::string &c);

  void show();

private:
  Console cons;
  int text_width;    // width of text accounting for padding
  int content_lines; // total number of lines of content
  int visible_lines; // total number of lines in view
  int overhead;      // number of lines reserved for header & footer

  void display();
  bool process_input(); /* return values:
   t: continue
   f: exit
   */

  void update_size();
};

class InfoPage {
public:
  termui::string title, content;

  InfoPage(const termui::string &t, const termui::string &c);
  InfoPage(const std::string &t, const std::string &c);

  void show();

private:
  Console cons;
  int line_cursor;   // current scroll value
  int text_width;    // width of text accounting for padding
  int content_lines; // total number of lines of content
  int visible_lines; // total number of lines in view
  int voh;           // vertical overhead (3 header + 2 footer)
  int hoh;           // horizontal overhead (2 left + 2 right)

  void display();
  bool process_input(); /* return values:
   t: continue
   f: exit
   */

  void update_size();
};

class InputBox {
public:
  termui::string field, response, placeholder;
  int w, h;

  InputBox(const termui::string &field, const termui::string &response, const termui::string &placeholder);
  InputBox(const std::string &field, const termui::string &response, const std::string &placeholder);

  std::shared_ptr<std::string> shareField() const;

  const std::string &getField() const;
  std::string &getField();

  std::shared_ptr<std::string> shareResponse() const;

  const std::string &getResponse() const;
  std::string &getResponse();

  void show();

private:
  Console cons;

  void display();
  bool process_input();

  void update_size();
};

class InputPage {
public:
  termui::string title;
  termui::strings fields, responses;

  int col;

  InputPage(const termui::string &title, const termui::strings &fields, const termui::strings &responses, int ls = 0, int col = clr::PASTELPINK);
  InputPage(const std::string &title, const std::vector<std::string> &fields, const termui::strings &responses, int ls = 0, int col = clr::PASTELPINK);

  // std::shared_ptr<std::vector<std::string>> shareFields() const;

  // const std::vector<std::string> &getFields() const;
  // std::vector<std::string> &getFields();

  // std::shared_ptr<std::vector<std::string>> shareResponses() const;

  // const std::vector<std::string> &getResponses() const;
  // std::vector<std::string> &getResponses();

  int show(); /* return values:
  -1: exit
  >=0: element selected (return value as index)
  */

private:
  Console cons;

  int cursor;          // current scroll value
  bool selected;       // element selection indicator
  int visible_lines;   // total number of lines in view
  int start_line;      // index value of first line which shows in view
  int line_seperation; // number of blank lines between elements
  int voh;             // vertical overhead (3 header + 2 footer)
  int hoh;             // horizontal overhead (2 left + 2 right)

  void display();
  int process_input(); /* return values:
  -1: exit
  0: continue
  1: element selected
  */

  void update_size();
};

class Menu {
public:
  termui::string title;
  termui::strings elements;

  int line_seperation;

  Menu(const termui::string &t, const termui::strings &e, int ls = 0);
  Menu(const std::string &t, const std::vector<std::string> &e, int ls = 0);

  // std::shared_ptr<List> shareList() const { return list; }

  // const std::vector<std::shared_ptr<item::ListItem>> &getElements() const { return (*list).getElements(); }
  // std::vector<std::shared_ptr<item::ListItem>> &getElements() { return (*list).getElements(); }

  int show(); // returns index of selected option

private:
  Console cons;
  List list;

  int cursor; // current scroll value
  int voh;    // vertical overhead
  int hoh;    // horizontal overhead

  void display();
  bool process_input();

  void update_size();
};

class MultiMenu {
public:
  termui::string title;
  termui::strings elements;
  int line_seperation;

  MultiMenu(const termui::string &t, const termui::strings &e, int ls = 0);
  MultiMenu(const std::string &t, const std::vector<std::string> &e, int ls = 0);

  bool isSelected(int i);

  int show(); /* return values:
  -1: exit
  >=0: element selected (return value as index)
  */

private:
  Console cons;
  SelectList list;

  int cursor; // current scroll value
  int voh;    // vertical overhead
  int hoh;    // horizontal overhead

  void display();
  bool process_input();

  void update_size();
};

class FancyMenu {
public:
  termui::string title;
  termui::strings text, desc;
  int line_seperation;

  FancyMenu(const termui::string &title, const termui::strings &texts, const termui::strings &descs, int ls = 1);
  FancyMenu(const std::string &title, const std::vector<std::string> &texts, const std::vector<std::string> &descs, int ls = 1);

  int show(); // returns index of selected option

private:
  Console cons;
  FancyList list;

  int cursor; // current scroll value
  int voh;    // vertical overhead
  int hoh;    // horizontal overhead

  void display();
  bool process_input();

  void update_size();
};

class BinaryMenu {
public:
  termui::string title, text, affirmative, negative;
  int w, h;

  BinaryMenu(const termui::string &title, const termui::string &text, const termui::string &affirmative, const termui::string &negative);
  BinaryMenu(const std::string &title, const std::string &text, const std::string &affirmative, const std::string &negative);

  bool show(); /* return values:
  t: affirmative
  f: negative
  */

private:
  Console cons;
  int status;   // indicates which button is highlighted
  bool reprint; // flag to indicate if a reprint is required (reduce flickering + wasted cycles)

  void display();
  bool process_input();

  void update_size();
};

class Editor {
public:
  termui::string title, content;

  Editor(const std::string &title, const termui::string &content);
  Editor(const termui::string &title, const termui::string &content);

  void show();

private:
  Console cons;
  int line_cursor;   // current scroll value
  int text_width;    // width of text accounting for padding
  int content_lines; // total number of lines of content
  int visible_lines; // total number of lines in view
  int voh;           // vertical overhead
  int hoh;           // horizontal overhead

  void display();
  bool process_input(); /* return values:
   t: continue
   f: exit
   */

  void update_size();
};

class Spreadsheet {
public:
  termui::string title;
  Table table;

  Spreadsheet(const termui::string &title, const termui::strings &cols, const std::vector<termui::strings> &data);
  Spreadsheet(const std::string &title, const std::vector<std::string> &cols, const std::vector<termui::strings> &data);

  void show();

private:
  Console cons;

  int voh;
  int hoh;

  void display();
  bool process_input(); /* return values:
   t: continue
   f: exit
   */

  void update_size();
};

} // namespace termui

#endif

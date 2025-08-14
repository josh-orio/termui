#ifndef INTERFACES_HPP
#define INTERFACES_HPP

#include "console.hpp"
#include "elements.hpp"
#include "structs.hpp"
#include "unicode.hpp"
#include "util.hpp"
#include <memory>
#include <vector>

namespace termui {

class InfoBox {
public:
  std::shared_ptr<std::string> title, content;
  int w, h;

  // still need to fill in width and height stuff here

  InfoBox();
  InfoBox(const std::string &t, const std::string &c);
  InfoBox(std::string &&t, std::string &&c);
  InfoBox(std::shared_ptr<std::string> sharedT, std::shared_ptr<std::string> sharedC);

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
  std::shared_ptr<std::string> title, content;

  InfoPage();
  InfoPage(const std::string &t, const std::string &c);
  InfoPage(std::string &&t, std::string &&c);
  InfoPage(std::shared_ptr<std::string> sharedT, std::shared_ptr<std::string> sharedC);

  void show();

private:
  Console cons;
  int line_cursor;   // current scroll value
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

class InputBox {
public:
  std::shared_ptr<std::string> field, response, placeholder;
  int w, h;

  InputBox();
  InputBox(std::string &f, std::string &r, std::string &p);
  InputBox(std::string &&f, std::string &r, std::string &&p);
  InputBox(std::shared_ptr<std::string> f, std::shared_ptr<std::string> r, std::shared_ptr<std::string> p);
  InputBox(std::string &&f, std::shared_ptr<std::string> r, std::string &&p);

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
  std::shared_ptr<const std::string> title;
  std::shared_ptr<std::vector<std::string>> fields;
  std::shared_ptr<std::vector<std::string>> responses;

  InputPage();
  InputPage(std::string &t, std::vector<std::string> &f, std::vector<std::string> &r, int ls = 0); // existing t + existing vectors of strings
  InputPage(std::string &t, std::shared_ptr<std::vector<std::string>> f, std::shared_ptr<std::vector<std::string>> r, int ls = 0); // shared t + .share()

  InputPage(std::string &&t, std::vector<std::string> &f, std::vector<std::string> &r, int ls = 0); // rvalue t + existing vectors of strings
  InputPage(std::string &&t, std::shared_ptr<std::vector<std::string>> f, std::shared_ptr<std::vector<std::string>> r, int ls = 0); // rvalue t + .share()

  InputPage(std::shared_ptr<std::string> t, std::vector<std::string> &f, std::vector<std::string> &r, int ls = 0); // shared t + existing vectors of strings
  InputPage(std::shared_ptr<std::string> t, std::shared_ptr<std::vector<std::string>> f, std::shared_ptr<std::vector<std::string>> r,
            int ls = 0); // all .share()

  std::shared_ptr<std::vector<std::string>> shareFields() const;

  const std::vector<std::string> &getFields() const;
  std::vector<std::string> &getFields();

  std::shared_ptr<std::vector<std::string>> shareResponses() const;

  const std::vector<std::string> &getResponses() const;
  std::vector<std::string> &getResponses();

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
  int overhead;        // number of lines reserved for header & footer

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
  std::shared_ptr<std::string> title;
  std::shared_ptr<List> list;

  int line_seperation;

  Menu();                                                                                           // vector instantiation
  Menu(std::string &t, std::vector<std::string> &e, int ls = 0);                                    // existing vector of strings
  Menu(std::string &t, std::vector<item::ListItem> &e, int ls = 0);                                 // existing vector of items
  Menu(std::string &&t, std::vector<std::string> &&e, int ls = 0);                                  // rvalue vec of str
  Menu(std::string &&t, std::vector<item::ListItem> &&e, int ls = 0);                               // rvalue vec of LI
  Menu(std::shared_ptr<std::string> t, std::shared_ptr<std::vector<std::string>> e, int ls = 0);    // .share()
  Menu(std::shared_ptr<std::string> t, std::shared_ptr<std::vector<item::ListItem>> e, int ls = 0); // from List.share()

  std::shared_ptr<List> shareList() const { return list; }

  const std::vector<item::ListItem> &getElements() const { return (*list).getElements(); }
  std::vector<item::ListItem> &getElements() { return (*list).getElements(); }

  int show(); // returns index of selected option

private:
  Console cons;
  int cursor; // current scroll value

  void display();
  bool process_input();

  void update_size();
};

class MultiMenu {
public:
  std::shared_ptr<std::string> title;
  std::shared_ptr<SelectList> list;
  int line_seperation;

  MultiMenu();
  MultiMenu(std::string &t, std::vector<std::string> &e, int ls = 0);
  MultiMenu(std::string &t, std::vector<item::MultiListItem> &e, int ls = 0);
  MultiMenu(std::string &&t, std::vector<std::string> &&e, int ls = 0);
  MultiMenu(std::string &&t, std::vector<item::MultiListItem> &&e, int ls = 0);
  MultiMenu(std::shared_ptr<std::string> t, std::shared_ptr<std::vector<std::string>> e, int ls = 0);
  MultiMenu(std::shared_ptr<std::string> t, std::shared_ptr<std::vector<item::MultiListItem>> e, int ls = 0);

  int show(); // returns index of selected option

private:
  Console cons;
  int cursor; // current scroll value
  int voh;    // vertical overhead
  int hoh;    // horizontal overhead

  void display();
  bool process_input();

  void update_size();
};

class FancyMenu {
public:
  std::shared_ptr<std::string> title;
  std::shared_ptr<FancyList> list;
  int line_seperation;

  FancyMenu();
  FancyMenu(std::string &t, std::vector<std::string> &e, std::vector<std::string> &d, int ls = 1);
  FancyMenu(std::string &t, std::vector<item::FancyListItem> &e, int ls = 1);
  FancyMenu(std::string &&t, std::vector<std::string> &&e, std::vector<std::string> &&d, int ls = 1);
  FancyMenu(std::string &&t, std::vector<item::FancyListItem> &&e, int ls = 1);
  FancyMenu(std::shared_ptr<std::string> t, std::shared_ptr<std::vector<std::string>> e, std::shared_ptr<std::vector<std::string>> d, int ls = 1);
  FancyMenu(std::shared_ptr<std::string> t, std::shared_ptr<std::vector<item::FancyListItem>> e, int ls = 1);

  int show(); // returns index of selected option

private:
  Console cons;
  int cursor; // current scroll value

  void display();
  bool process_input();

  void update_size();
};

class SpreadSheet {};

class Editor {
public:
  const std::string &title;
  std::string &text;

  Editor(const std::string &title, std::string &text);

  void show();

private:
  Console cons;
  int cursor;
  int overhead;
  int visible_lines;

  void display();
  bool process_input();
};

class BinaryMenu {
public:
  std::shared_ptr<std::string> title, text, affirmative, negative;
  int w, h;

  BinaryMenu();
  BinaryMenu(const std::string &t, const std::string &x, const std::string &aff, const std::string &neg);
  BinaryMenu(std::string &&t, std::string &&x, std::string &&aff, std::string &&neg);
  BinaryMenu(std::shared_ptr<std::string> sharedT, std::shared_ptr<std::string> sharedX, std::shared_ptr<std::string> sharedAff,
             std::shared_ptr<std::string> sharedNeg);

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

} // namespace termui

#endif

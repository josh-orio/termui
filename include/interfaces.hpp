#ifndef INTERFACES_HPP
#define INTERFACES_HPP

#include "console.hpp"
#include "elements.hpp"
#include "nlohmann/json.hpp"
#include "util.hpp"
#include <vector>

// interfaces should not own data, rather 'wrap' around existing data structures and provide controlled
// modification/access to data inside the program

namespace termui {

class Info {
public:
  Console cons;
  const std::string title, content;

  Info(const std::string &t, const std::string &c);

  void show();

private:
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

class Input {
public:
  Console cons;
  const std::string title;
  const std::vector<std::string> fields;
  std::vector<std::string> responses;

  Input(const std::string &t, const std::vector<std::string> &f, const std::vector<std::string> &r, int ls = 0);

  int show(); /* return values:
  -1: exit
  >=0: element selected (return value as index)
  */

private:
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
  Console cons;
  const std::string title;
  const std::vector<std::string> options;

  Menu(const std::string &t, const std::vector<std::string> &o, int ls = 0);

  int show(); // returns index of selected option

private:
  int cursor;          // current scroll value
  int visible_lines;   // total number of lines in view
  int start_line;      // index value of first line which shows in view
  int line_seperation; // number of blank lines between elements
  int overhead;        // number of lines reserved for header & footer

  void display();
  bool process_input();

  void update_size();
};

class BinaryMenu {
public:
  std::string title, text, affirmative, negative;
  int w, h;

  BinaryMenu(std::string title, std::string text, std::string affirmative, std::string negative, int w, int h);

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
};

} // namespace termui

#endif

#ifndef INTERFACES_HPP
#define INTERFACES_HPP

#include "console.hpp"
#include "formatting.hpp"
#include "nlohmann/json.hpp"
#include <vector>

class Info {
public:
  Console cons;
  std::string title, content;

  Info(std::string t, std::string c);

  void show();

private:
  int line_cursor;   // current scroll value
  int text_width;    // width of text accounting for padding
  int content_lines; // total number of lines of content
  int visible_lines; // total number of lines in view
  int overhead;      // number of lines reserved for header & footer

  void display();
  bool await_input();

  void update_size();
};

class Input {
public:
  Console cons;
  std::string title;
  std::vector<std::string> fields, responses;

  Input(std::string t, std::vector<std::string> f);
  Input(std::string t, std::vector<std::string>, int ls);

  int show();
  std::vector<std::string> get_responses();

private:
  int cursor;          // current scroll value
  bool selected;       // element selection indicator
  int visible_lines;   // total number of lines in view
  int start_line;      // index value of first line which shows in view
  int line_seperation; // number of blank lines between elements
  int overhead;        // number of lines reserved for header & footer

  void display();
  int await_input();

  void update_size();
};

class Menu {
public:
  Console cons;
  std::string title;
  std::vector<std::string> options;

  Menu(std::string t, std::vector<std::string> o);
  Menu(std::string t, std::vector<std::string> o, int ls);

  int show(); // returns index of selected option

private:
  int cursor;          // current scroll value
  int visible_lines;   // total number of lines in view
  int start_line;      // index value of first line which shows in view
  int line_seperation; // number of blank lines between elements
  int overhead;        // number of lines reserved for header & footer

  void display();
  bool await_input();

  void update_size();
};

class Table {
public:
  Console cons;
  std::string title;
  std::vector<std::string> columns;
  std::vector<nlohmann::json> data;

  Table(std::string t, std::vector<std::string> c,
        std::vector<nlohmann::json> d);
  Table(std::string t, std::vector<std::string> c,
        std::vector<nlohmann::json> d, int ch, int ls);

  int show();

private:
  int cursor;          // current scroll value
  int visible_rows;    // number of table rows that will fit onto the terminal
  int start_line;      // index value of first row which shows in view
  int table_width;     // width of table in characters
  int column_width;    // width of each column in characters
  int line_seperation; // number of blank lines between elements
  int cell_height;     // number of lines dedicated to a cell
  int overhead;        // number of lines reserved for header & footer

  void display();
  int await_input();

  void update_size();
};

#endif

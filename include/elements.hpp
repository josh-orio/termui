/* UI elements */

#ifndef ELEMENTS_HPP
#define ELEMENTS_HPP

#include "formatting.hpp"
#include "util.hpp"
#include <iostream>
#include <string>

// elements should not be aware of position in terminal, only of their own properties such as width and height
// using absolute cursor positioning is prohibited
// using relative cursor positioning is encouraged

namespace termui {

class Box {
public:
  int w, h;
  std::string color;

  Box(int w, int h, std::string color);

  std::string render();
};

class Button {
public:
  int w;
  bool selected;
  std::string text;
  std::string active_color;

  Button(int w, std::string text, std::string active_color);

  std::string render();
};

class ProgressBar {
public:
  int width;
  float completion;
  std::string fg_col, bg_col;

  ProgressBar(int width, float completion, std::string fg_col, std::string bg_col);

  std::string render();
};

} // namespace termui

#endif
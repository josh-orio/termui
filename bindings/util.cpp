#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <util.hpp>

namespace py = pybind11;

void bind_util(py::module_ &m) {
  // colors
  py::module_ clr = m.def_submodule("clr");
  clr.attr("DEFAULT") = termui::clr::DEFAULT;
  clr.attr("RED") = termui::clr::RED;
  clr.attr("GREEN") = termui::clr::GREEN;
  clr.attr("BLUE") = termui::clr::BLUE;
  clr.attr("YELLOW") = termui::clr::YELLOW;
  clr.attr("ORANGE") = termui::clr::ORANGE;
  clr.attr("PINK") = termui::clr::PINK;
  clr.attr("MAGENTA") = termui::clr::MAGENTA;
  clr.attr("PURPLE") = termui::clr::PURPLE;
  clr.attr("LIME") = termui::clr::LIME;
  clr.attr("TEAL") = termui::clr::TEAL;
  clr.attr("BLACK") = termui::clr::BLACK;
  clr.attr("WHITE") = termui::clr::WHITE;
  clr.attr("BLUEPURPLE") = termui::clr::BLUEPURPLE;
  clr.attr("NEONY_PURPLE") = termui::clr::NEONY_PURPLE;
  clr.attr("MIDBLUE") = termui::clr::MIDBLUE;
  clr.attr("DARKGREY") = termui::clr::DARKGREY;
  clr.attr("LIGHTGREY") = termui::clr::LIGHTGREY;
  clr.attr("PASTELPINK") = termui::clr::PASTELPINK;
  clr.attr("LIGHTBLUE") = termui::clr::LIGHTBLUE;
  clr.attr("LIGHTPURPLE") = termui::clr::LIGHTPURPLE;

  // functions
  m.def("fg_apply", &termui::fg_apply);
  m.def("bg_apply", &termui::bg_apply);
  m.def("whitespace", &termui::whitespace);
  m.def("horizontal_line", &termui::horizontal_line, py::arg("len"), py::arg("col") = termui::clr::DEFAULT);
  m.def("vertical_line", &termui::vertical_line, py::arg("len"), py::arg("col") = termui::clr::DEFAULT);
  m.def("line_sep", &termui::line_sep);
  m.def("bold_text", &termui::bold_text);
  m.def("reverse_video", &termui::reverse_video);
  m.def("faint_text", &termui::faint_text);
  m.def("curs_up", &termui::curs_up);
  m.def("curs_down", &termui::curs_down);
  m.def("curs_right", &termui::curs_right);
  m.def("curs_left", &termui::curs_left);
  m.def("visible_length", &termui::visible_length);
  m.def("max_visible_length", &termui::max_visible_length);

  // key submodule
  py::module_ key = m.def_submodule("key");
  key.attr("ENTER") = termui::key::ENTER;
  key.attr("ESC") = termui::key::ESC;
  key.attr("DEL") = termui::key::DEL;
  key.attr("U_ARROW") = termui::key::U_ARROW;
  key.attr("D_ARROW") = termui::key::D_ARROW;
  key.attr("R_ARROW") = termui::key::R_ARROW;
  key.attr("L_ARROW") = termui::key::L_ARROW;
  key.attr("SHIFT_U_ARROW") = termui::key::SHIFT_U_ARROW;
  key.attr("SHIFT_D_ARROW") = termui::key::SHIFT_D_ARROW;
  key.attr("SHIFT_R_ARROW") = termui::key::SHIFT_R_ARROW;
  key.attr("SHIFT_L_ARROW") = termui::key::SHIFT_L_ARROW;

  // unicode submodule
  py::module_ unicode = m.def_submodule("unicode");
  unicode.attr("TICK") = termui::unicode::TICK;
  unicode.attr("DOT") = termui::unicode::DOT;
  unicode.attr("VERTICAL") = termui::unicode::VERTICAL;
  unicode.attr("VERTICAL_WIDE") = termui::unicode::VERTICAL_WIDE;
  unicode.attr("TOP_LEFT") = termui::unicode::TOP_LEFT;
  unicode.attr("TOP_RIGHT") = termui::unicode::TOP_RIGHT;
  unicode.attr("BOTTOM_LEFT") = termui::unicode::BOTTOM_LEFT;
  unicode.attr("BOTTOM_RIGHT") = termui::unicode::BOTTOM_RIGHT;
  unicode.attr("TOP_LEFT_ROUND") = termui::unicode::TOP_LEFT_ROUND;
  unicode.attr("TOP_RIGHT_ROUND") = termui::unicode::TOP_RIGHT_ROUND;
  unicode.attr("BOTTOM_LEFT_ROUND") = termui::unicode::BOTTOM_LEFT_ROUND;
  unicode.attr("BOTTOM_RIGHT_ROUND") = termui::unicode::BOTTOM_RIGHT_ROUND;
  unicode.attr("ELLIPSIS") = termui::unicode::ELLIPSIS;
  unicode.attr("L_ARROW") = termui::unicode::L_ARROW;
  unicode.attr("R_ARROW") = termui::unicode::R_ARROW;
  unicode.attr("U_ARROW") = termui::unicode::U_ARROW;
  unicode.attr("D_ARROW") = termui::unicode::D_ARROW;
  unicode.attr("ENTER") = termui::unicode::ENTER;
  unicode.attr("TAB") = termui::unicode::TAB;
  unicode.attr("ESC") = termui::unicode::ESC;
}

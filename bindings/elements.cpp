#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <elements.hpp>

namespace py = pybind11;

void bind_elements(py::module_ &m) {
  // --- Text
  py::class_<termui::Text>(m, "Text")
      .def(py::init<>())
      .def(py::init<const termui::string &, int, int, int, int>(), py::arg("t"), py::arg("w"), py::arg("h"), py::arg("fg_col") = termui::clr::DEFAULT,
           py::arg("bg_col") = termui::clr::DEFAULT)
      .def(py::init<const std::string &, int, int, int, int>(), py::arg("t"), py::arg("w"), py::arg("h"), py::arg("fg_col") = termui::clr::DEFAULT,
           py::arg("bg_col") = termui::clr::DEFAULT)
      //   .def(py::init<std::shared_ptr<std::string> &, int, int, int, int>(), py::arg("t"), py::arg("w"), py::arg("h"), py::arg("fg_col") =
      //   termui::clr::DEFAULT,
      //        py::arg("bg_col") = termui::clr::DEFAULT)
      //   .def("share", &termui::Text::share)
      //   .def("getText", py::overload_cast<>(&termui::Text::getText, py::const_))
      //   .def("getText", py::overload_cast<>(&termui::Text::getText)) // not impl
      .def("render", &termui::Text::render)
      .def_readwrite("w", &termui::Text::w)
      .def_readwrite("h", &termui::Text::h)
      .def_readwrite("fg_col", &termui::Text::fg_col)
      .def_readwrite("bg_col", &termui::Text::bg_col);

  // --- Input
  py::class_<termui::Input>(m, "Input")
      .def(py::init<>())
      .def(py::init<const termui::string &, const termui::string &, int, int, int>(), py::arg("val"), py::arg("plh"), py::arg("w"),
           py::arg("valCol") = termui::clr::DEFAULT, py::arg("plhCol") = termui::clr::LIGHTGREY)
      .def(py::init<const termui::string &, const std::string &, int, int, int>(), py::arg("val"), py::arg("plh"), py::arg("w"),
           py::arg("valCol") = termui::clr::DEFAULT, py::arg("plhCol") = termui::clr::LIGHTGREY)
      .def(py::init<const termui::string &, int, int>(), py::arg("val"), py::arg("w"), py::arg("valCol") = termui::clr::DEFAULT)
      //   .def(py::init<std::shared_ptr<std::string>, std::shared_ptr<std::string>, int, int, int>(), py::arg("val"), py::arg("plh"), py::arg("w"),
      //        py::arg("valCol") = termui::clr::DEFAULT, py::arg("plhCol") = termui::clr::LIGHTGREY)
      //   .def(py::init<std::shared_ptr<std::string>, const std::string &, int, int, int>(), py::arg("val"), py::arg("plh"), py::arg("w"),
      //        py::arg("valCol") = termui::clr::DEFAULT, py::arg("plhCol") = termui::clr::LIGHTGREY)
      //   .def(py::init<std::shared_ptr<std::string>, int, int>(), py::arg("val"), py::arg("w"), py::arg("valCol") = termui::clr::DEFAULT)
      //   .def("shareVal", &termui::Input::shareVal)
      //   .def("sharePlh", &termui::Input::sharePlh)
      .def("getVal", py::overload_cast<>(&termui::Input::getVal, py::const_))
      .def("getVal", py::overload_cast<>(&termui::Input::getVal))
      .def("getPlh", py::overload_cast<>(&termui::Input::getPlh, py::const_))
      .def("getPlh", py::overload_cast<>(&termui::Input::getPlh))
      .def("render", &termui::Input::render)
      .def_readwrite("w", &termui::Input::w)
      .def_readwrite("valCol", &termui::Input::valCol)
      .def_readwrite("plhCol", &termui::Input::plhCol);

  // --- Button
  py::class_<termui::Button>(m, "Button")
      .def(py::init<>())
      .def(py::init<const termui::string &, int, int, int>())
      .def(py::init<const std::string &, int, int, int>())
      //   .def("shareText", &termui::Button::shareText)
      .def("getText", py::overload_cast<>(&termui::Button::getText, py::const_))
      .def("getText", py::overload_cast<>(&termui::Button::getText))
      .def("render", &termui::Button::render)
      .def_readwrite("w", &termui::Button::w)
      .def_readwrite("selected", &termui::Button::selected)
      .def_readwrite("active_color", &termui::Button::active_color)
      .def_readwrite("idle_color", &termui::Button::idle_color);

  // --- List
  py::class_<termui::List>(m, "List")
      .def(py::init<const termui::strings &, int, int, int, int>(), py::arg("strs"), py::arg("w"), py::arg("h"), py::arg("ls") = 0,
           py::arg("col") = termui::clr::PASTELPINK)
      .def("getStrings", py::overload_cast<>(&termui::List::getStrings, py::const_), py::return_value_policy::reference_internal)
      .def("getStrings", py::overload_cast<>(&termui::List::getStrings), py::return_value_policy::reference_internal)
      .def("getElement", py::overload_cast<int>(&termui::List::getElement, py::const_), py::return_value_policy::reference_internal)
      .def("getElement", py::overload_cast<int>(&termui::List::getElement), py::return_value_policy::reference_internal)
      .def("render", &termui::List::render)
      .def("cursor_up", &termui::List::cursor_up)
      .def("cursor_down", &termui::List::cursor_down)
      .def_readwrite("w", &termui::List::w)
      .def_readwrite("h", &termui::List::h)
      .def_readwrite("cursor", &termui::List::cursor)
      .def_readwrite("line_spacing", &termui::List::line_spacing)
      .def_readwrite("active_color", &termui::List::active_color);

  // --- SelectList
  py::class_<termui::SelectList>(m, "SelectList")
      .def(py::init<const termui::strings &, int, int, int, int>(), py::arg("strs"), py::arg("w"), py::arg("h"), py::arg("ls") = 0,
           py::arg("col") = termui::clr::PASTELPINK)
      .def("getStrings", py::overload_cast<>(&termui::SelectList::getStrings, py::const_), py::return_value_policy::reference_internal)
      .def("getStrings", py::overload_cast<>(&termui::SelectList::getStrings), py::return_value_policy::reference_internal)
      .def("getElement", py::overload_cast<int>(&termui::SelectList::getElement, py::const_), py::return_value_policy::reference_internal)
      .def("getElement", py::overload_cast<int>(&termui::SelectList::getElement), py::return_value_policy::reference_internal)
      .def("getSelection", &termui::SelectList::getSelection)
      .def("render", &termui::SelectList::render)
      .def("cursor_up", &termui::SelectList::cursor_up)
      .def("cursor_down", &termui::SelectList::cursor_down)
      .def("select", &termui::SelectList::select)
      .def_readwrite("w", &termui::SelectList::w)
      .def_readwrite("h", &termui::SelectList::h)
      .def_readwrite("cursor", &termui::SelectList::cursor)
      .def_readwrite("line_spacing", &termui::SelectList::line_spacing)
      .def_readwrite("active_color", &termui::SelectList::active_color);

  // --- FancyList
  py::class_<termui::FancyList>(m, "FancyList")
      .def(py::init<const termui::strings &, const termui::strings &, int, int, int, int>(), py::arg("tstrs"), py::arg("dstrs"), py::arg("w"), py::arg("h"),
           py::arg("ls") = 1, py::arg("col") = termui::clr::PASTELPINK)
      .def("getText", py::overload_cast<int>(&termui::FancyList::getText, py::const_), py::return_value_policy::reference_internal)
      .def("getText", py::overload_cast<int>(&termui::FancyList::getText), py::return_value_policy::reference_internal)
      .def("getDesc", py::overload_cast<int>(&termui::FancyList::getDesc, py::const_), py::return_value_policy::reference_internal)
      .def("getDesc", py::overload_cast<int>(&termui::FancyList::getDesc), py::return_value_policy::reference_internal)
      .def("render", &termui::FancyList::render)
      .def("cursor_up", &termui::FancyList::cursor_up)
      .def("cursor_down", &termui::FancyList::cursor_down)
      .def_readwrite("w", &termui::FancyList::w)
      .def_readwrite("h", &termui::FancyList::h)
      .def_readwrite("cursor", &termui::FancyList::cursor)
      .def_readwrite("line_spacing", &termui::FancyList::line_spacing)
      .def_readwrite("active_color", &termui::FancyList::active_color);

  // --- Table
  py::class_<termui::Table>(m, "Table")
      .def(py::init<const termui::strings &, const std::vector<termui::strings> &, std::vector<int>, int, int, int, int>(), py::arg("cols"), py::arg("rows"),
           py::arg("colwidths"), py::arg("table_height"), py::arg("cell_height") = 1, py::arg("line_seperation") = 0, py::arg("col") = termui::clr::BLUEPURPLE)
      .def("getCols", py::overload_cast<>(&termui::Table::getCols, py::const_), py::return_value_policy::reference_internal)
      .def("getCols", py::overload_cast<>(&termui::Table::getCols), py::return_value_policy::reference_internal)
      .def("colWidth", py::overload_cast<int>(&termui::Table::colWidth, py::const_))
      .def("colWidth", py::overload_cast<int>(&termui::Table::colWidth))
      .def("getRow", py::overload_cast<int>(&termui::Table::getRow, py::const_), py::return_value_policy::reference_internal)
      .def("getRow", py::overload_cast<int>(&termui::Table::getRow), py::return_value_policy::reference_internal)
      .def("getCell", py::overload_cast<int, int>(&termui::Table::getCell, py::const_), py::return_value_policy::reference_internal)
      .def("getCell", py::overload_cast<int, int>(&termui::Table::getCell), py::return_value_policy::reference_internal)
      .def("colCount", &termui::Table::colCount)
      .def("rowCount", &termui::Table::rowCount)
      .def("render", &termui::Table::render)
      .def("cursor_up", &termui::Table::cursor_up)
      .def("cursor_down", &termui::Table::cursor_down)
      .def_readwrite("cursor", &termui::Table::cursor)
      .def_readwrite("line_seperation", &termui::Table::line_seperation)
      .def_readwrite("cell_height", &termui::Table::cell_height)
      .def_readwrite("active_color", &termui::Table::active_color)
      .def_readwrite("table_height", &termui::Table::table_height)
      .def_readwrite("table_width", &termui::Table::table_width);

  // --- Box
  py::class_<termui::Box>(m, "Box")
      .def(py::init<int, int, int>())
      .def("render", &termui::Box::render)
      .def_readwrite("w", &termui::Box::w)
      .def_readwrite("h", &termui::Box::h)
      .def_readwrite("color", &termui::Box::color);

  // --- ProgressBar
  py::class_<termui::ProgressBar>(m, "ProgressBar")
      .def(py::init<int, float, int, int>())
      .def("render", &termui::ProgressBar::render)
      .def_readwrite("width", &termui::ProgressBar::width)
      .def_readwrite("completion", &termui::ProgressBar::completion)
      .def_readwrite("fg_col", &termui::ProgressBar::fg_col)
      .def_readwrite("bg_col", &termui::ProgressBar::bg_col);

  // --- DashedArea
  py::class_<termui::DashedArea>(m, "DashedArea")
      .def(py::init<int, int, int>())
      .def("render", &termui::DashedArea::render)
      .def_readwrite("w", &termui::DashedArea::w)
      .def_readwrite("h", &termui::DashedArea::h)
      .def_readwrite("col", &termui::DashedArea::col);
}

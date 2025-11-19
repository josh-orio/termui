#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <interfaces.hpp>

namespace py = pybind11;

void bind_interfaces(py::module_ &m) {
  m.doc() = "Python bindings for termui interfaces";

  py::enum_<termui::state>(m, "State").value("EXIT", termui::state::EXIT).value("CONTINUE", termui::state::CONTINUE).value("SELECT", termui::state::SELECT);

  // --- InfoBox
  py::class_<termui::InfoBox>(m, "InfoBox")
      .def(py::init<const termui::string &, const termui::string &>())
      .def(py::init<const std::string &, const std::string &>())
      .def("show", &termui::InfoBox::show);

  // --- InfoPage
  py::class_<termui::InfoPage>(m, "InfoPage")
      .def(py::init<const termui::string &, const termui::string &>())
      .def(py::init<const std::string &, const std::string &>())
      .def("show", &termui::InfoPage::show);

  // --- InputBox
  py::class_<termui::InputBox>(m, "InputBox")
      .def(py::init<const termui::string &, const termui::string &, const termui::string &>())
      .def(py::init<const std::string &, const termui::string &, const std::string &>())
      .def("show", &termui::InputBox::show)
      .def("field", [](termui::InputBox &b) { return b.getField(); })
      .def("response", [](termui::InputBox &b) { return b.getResponse(); });

  // --- InputPage
  py::class_<termui::InputPage>(m, "InputPage")
      .def(py::init<const termui::string &, const termui::strings &, const termui::strings &, int, int>())
      .def(py::init<const std::string &, const termui::strings &, const termui::strings &, int, int>())
      .def(py::init<const std::string &, const std::vector<std::string> &, const termui::strings &, int, int>())
      .def("show", &termui::InputPage::show);

  // --- Menu
  py::class_<termui::Menu>(m, "Menu")
      .def(py::init<const termui::string &, const termui::strings &, int>())
      .def(py::init<const std::string &, const termui::strings &, int>())
      .def(py::init<const std::string &, const std::vector<std::string> &, int>())
      .def("show", &termui::Menu::show)
      .def("cursor", &termui::Menu::cursor)
      .def("status", &termui::Menu::status);

  // --- MultiMenu
  py::class_<termui::MultiMenu>(m, "MultiMenu")
      .def(py::init<const termui::string &, const termui::strings &, int>())
      .def(py::init<const std::string &, const termui::strings &, int>())
      .def(py::init<const std::string &, const std::vector<std::string> &, int>())
      .def("show", &termui::MultiMenu::show)
      .def("cursor", &termui::MultiMenu::cursor)
      .def("status", &termui::MultiMenu::status)
      .def("isSelected", &termui::MultiMenu::isSelected);

  // --- FancyMenu
  py::class_<termui::FancyMenu>(m, "FancyMenu")
      .def(py::init<const termui::string &, const termui::strings &, const termui::strings &, int>())
      .def(py::init<const std::string &, const termui::strings &, const termui::strings &, int>())
      .def(py::init<const std::string &, const std::vector<std::string> &, const std::vector<std::string> &, int>())
      .def("show", &termui::FancyMenu::show)
      .def("cursor", &termui::FancyMenu::cursor)
      .def("status", &termui::FancyMenu::status);

  // --- BinaryMenu
  py::class_<termui::BinaryMenu>(m, "BinaryMenu")
      .def(py::init<const termui::string &, const termui::string &, const termui::string &, const termui::string &>())
      .def(py::init<const std::string &, const std::string &, const std::string &, const std::string &>())
      .def("show", &termui::BinaryMenu::show);

  // --- Editor
  py::class_<termui::Editor>(m, "Editor")
      .def(py::init<const termui::string &, const termui::string &>())
      .def(py::init<const std::string &, const termui::string &>())
      .def("show", &termui::Editor::show);

  // --- Spreadsheet
  py::class_<termui::Spreadsheet>(m, "Spreadsheet")
      .def(py::init<const termui::string &, const termui::strings &, const std::vector<termui::strings> &>())
      .def(py::init<const std::string &, const termui::strings &, const std::vector<termui::strings> &>())
      .def(py::init<const std::string &, const std::vector<std::string> &, const std::vector<termui::strings> &>())
      .def("show", &termui::Spreadsheet::show);
}

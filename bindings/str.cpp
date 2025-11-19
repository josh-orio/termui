#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <str.hpp>

namespace py = pybind11;

void bind_tstr(py::module_ &m) {
  py::class_<termui::string>(m, "String")
      .def(py::init<>())
      .def(py::init<const std::string &>())
      .def(
          "text_const", [](const termui::string &s) -> const std::string & { return s.text(); }, py::return_value_policy::reference_internal)
      .def(
          "text", [](termui::string &s) -> std::string & { return s.text(); }, py::return_value_policy::reference_internal);

  py::class_<termui::strings>(m, "Strings")
      .def(py::init<>())
      .def(py::init<const std::vector<std::string> &>())
      .def(py::init<const termui::strings &>())
      .def(py::init<int, const std::string &>())
      .def(
          "get_item_const", [](const termui::strings &s, int i) -> const std::string & { return s.getItem(i); }, py::return_value_policy::reference_internal)
      .def(
          "get_item", [](termui::strings &s, int i) -> std::string & { return s.getItem(i); }, py::return_value_policy::reference_internal);
}

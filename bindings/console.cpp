#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <console.hpp>

namespace py = pybind11;

void bind_console(py::module_ &m) {
  py::class_<termui::BufferModeToggle>(m, "BufferModeToggle")
      .def(py::init<>())
      .def("off", &termui::BufferModeToggle::off)
      .def("on", &termui::BufferModeToggle::on);

  py::class_<termui::EchoModeToggle>(m, "EchoModeToggle").def(py::init<>()).def("off", &termui::EchoModeToggle::off).def("on", &termui::EchoModeToggle::on);

  py::class_<termui::CursorModeToggle>(m, "CursorModeToggle")
      .def(py::init<>())
      .def("off", &termui::CursorModeToggle::off)
      .def("on", &termui::CursorModeToggle::on);

  py::class_<termui::AlternateBufferToggle>(m, "AlternateBufferToggle")
      .def(py::init<>())
      .def("enable", &termui::AlternateBufferToggle::enable)
      .def("disable", &termui::AlternateBufferToggle::disable);

  py::class_<termui::MouseReportingToggle>(m, "MouseReportingToggle")
      .def(py::init<>())
      .def("enable", &termui::MouseReportingToggle::enable)
      .def("disable", &termui::MouseReportingToggle::disable);

  py::enum_<termui::MouseEventType>(m, "MouseEventType")
      .value("SCROLL_UP", termui::MouseEventType::SCROLL_UP)
      .value("SCROLL_DOWN", termui::MouseEventType::SCROLL_DOWN)
      .value("LEFT_CLICK", termui::MouseEventType::LEFT_CLICK)
      .value("MIDDLE_CLICK", termui::MouseEventType::MIDDLE_CLICK)
      .value("RIGHT_CLICK", termui::MouseEventType::RIGHT_CLICK)
      .value("BUTTON_RELEASE", termui::MouseEventType::BUTTON_RELEASE)
      .value("MOVE", termui::MouseEventType::MOVE);

  py::class_<termui::MouseInteraction>(m, "MouseInteraction")
      .def(py::init<std::string>())
      .def_readwrite("valid", &termui::MouseInteraction::valid)
      .def_readwrite("col", &termui::MouseInteraction::col)
      .def_readwrite("row", &termui::MouseInteraction::row)
      .def_readwrite("event", &termui::MouseInteraction::event)
      .def("match", py::overload_cast<termui::MouseEventType>(&termui::MouseInteraction::match))
      .def("match", py::overload_cast<termui::MouseEventType, size_t, size_t, size_t, size_t>(&termui::MouseInteraction::match));

  py::class_<termui::Console>(m, "Console")
      .def(py::init<bool, bool, bool, bool, bool>(), py::arg("buffered") = false, py::arg("echos") = false, py::arg("cursor") = false, py::arg("alt") = true,
           py::arg("mouse") = true)
      .def_readwrite("width", &termui::Console::width)
      .def_readwrite("height", &termui::Console::height)
      .def_readwrite("outbuff", &termui::Console::outbuff)
      .def_readwrite("buffered", &termui::Console::buffered)
      .def_readwrite("echos", &termui::Console::echos)
      .def_readwrite("cursor", &termui::Console::cursor)
      .def_readwrite("altterm", &termui::Console::altterm)
      .def_readwrite("mouserep", &termui::Console::mouserep)
      .def("show", &termui::Console::show)
      .def("close", &termui::Console::close)
      .def("update_size", &termui::Console::update_size)
      .def("print", py::overload_cast<int, int, std::string>(&termui::Console::print))
      .def("print", py::overload_cast<std::string>(&termui::Console::print))
      .def("clear_outbuff", &termui::Console::clear_outbuff)
      .def("clear_screen", &termui::Console::clear_screen)
      .def("clear_scrollback", &termui::Console::clear_scrollback)
      .def("flush", &termui::Console::flush, py::arg("s_clr") = true, py::arg("sb_clr") = true)
      .def("poll_input", &termui::Console::poll_input);
}

#include <pybind11/pybind11.h>

#include <termui.hpp>

namespace py = pybind11;

void bind_tstr(py::module_ &);
void bind_util(py::module_ &);
void bind_ansi(py::module_ &);
void bind_console(py::module_ &m);
void bind_elements(py::module_ &);
void bind_interfaces(py::module_ &m);

PYBIND11_MODULE(termui, m) {
  m.doc() = "termui bindings";
  bind_tstr(m);
  bind_util(m);
  bind_ansi(m);
  bind_console(m);
  bind_elements(m);
  bind_interfaces(m);
}

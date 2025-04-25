//PyBind11 wrapper
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "volsweeper.hpp"   // from include/
//#include "../../include/volsweeper.hpp"

namespace py = pybind11;

PYBIND11_MODULE(volsweeper_binding, m) {
    m.doc() = "VolSweeper board logic binding";

// After (using the real struct name)
py::class_<Minefield>(m, "Minefield")
    .def(py::init<std::string,size_t,size_t>())
    .def("increment_grid_entry", &Minefield::increment_grid_entry)
    .def("debug_output_string", &Minefield::debug_output_string)
    .def("output_string", &Minefield::output_string)
    .def("reveal_square", &Minefield::reveal_square)
    .def("get_grid", [](Minefield &m) { return m.grid; })
    .def("get_revealed", [](Minefield &m){
	return m.revealed;
	});
}


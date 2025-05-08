//PyBind11 wrapper
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "volsweeper.hpp"   // from include/

namespace py = pybind11;

PYBIND11_MODULE(volsweeper_binding, m) {
    m.doc() = "VolSweeper board logic binding";

py::class_<Minefield>(m, "Minefield")
    .def(py::init<std::string,size_t,size_t, std::pair<size_t,size_t>>())
    .def("output_string", &Minefield::output_string) 		//returns string stream
    .def("reveal_square", &Minefield::reveal_square) 		//reveal 0 
    .def("get_grid", [](Minefield &m) { return m.grid; }) 	//calls constructor
    .def("get_revealed", [](Minefield &m){return m.revealed;})
    .def("flag_square", &Minefield::flag_square)
    .def("chord", &Minefield::chord)
    .def("pattern_recognition", &pattern_recognition)
    .def("B1", &B1)
    .def("B2", &B2);
}



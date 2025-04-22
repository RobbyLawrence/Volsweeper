#include <pybind11/pybind11.h>
#include "../include/Volsweeper.hpp"

namespace py = pybind11;

PYBIND11_MODULE(board_logic, m) {
    // expose your Minefield struct/class
    py::class_<Minefield>(m, "Minefield")
        .def(py::init<size_t, size_t>(),
             py::arg("size"), py::arg("num_mines"))
        .def("generate", &Minefield::generate)            // if you have a generate() 
        .def("get_grid", &Minefield::get_grid);           // return vector<vector<int>>
}


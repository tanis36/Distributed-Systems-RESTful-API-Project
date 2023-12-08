#include <pybind11/pybind11.h>

#include "KeyStorage.h"

namespace py = pybind11;

PYBIND11_MODULE(keystorage, m) {
    py::class_<KeyStorage>(m, "KeyStorage")
        .def(py::init<>())
        .def("initialize", &KeyStorage::initialize)
        .def("create_kvs", &KeyStorage::create_kvs)
        .def("add", &KeyStorage::add)
        .def("remove", &KeyStorage::remove)
        .def("update", &KeyStorage::update)
        .def("find", &KeyStorage::find);
}
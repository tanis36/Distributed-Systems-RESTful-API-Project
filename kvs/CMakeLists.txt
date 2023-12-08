cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 17)

set(site-packages "${CMAKE_SOURCE_DIR}/../.venv/lib/python3.10/site-packages")

# we have to tell CMake where the pybind11 installation is. TODO: Find a better way to do this.
list(APPEND CMAKE_PREFIX_PATH "${site-packages}/pybind11/share/cmake/pybind11")

project(kvs LANGUAGES CXX)

find_package(Python 3.10 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(keystorage binding.cpp KeyStorage.cpp FlatFileStorageDevice.cpp)

install(TARGETS keystorage
        LIBRARY DESTINATION ${site-packages}
)
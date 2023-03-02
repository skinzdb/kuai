cmake_minimum_required(VERSION 3.4...3.18)
project(pysmong)

option(PYBIND11_FINDPYTHON ON)

# target_include_directories(pysmong PRIVATE ${YOUR_DIRECTORY})

# add_library(pysmong STATIC src/smongine.cpp)
# set_target_properties(smongine PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_subdirectory(vendor/pybind11)
pybind11_add_module(pysmong src/Main.cpp)

# target_link_libraries(cmake_example PRIVATE pysmong)

# # EXAMPLE_VERSION_INFO is defined by setup.py and passed into the C++ code as a
# # define (VERSION_INFO) here.
# target_compile_definitions(cmake_example
#                            PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})

cmake_minimum_required(VERSION 3.16)

project(pykuai)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(PYBIND11_FINDPYTHON ON)

# set_target_properties(kuai PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../kuai ${CMAKE_CURRENT_BINARY_DIR}/kuai)
add_subdirectory(vendor/pybind11)

pybind11_add_module(pykuai src/Main.cpp)

target_include_directories(${PROJECT_NAME}
    PUBLIC "${PROJECT_BINARY_DIR}"
    PUBLIC ../kuai/src
    PUBLIC ../kuai/vendor
    PUBLIC ../kuai/vendor/glm
    PUBLIC ../kuai/vendor/spdlog/include
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE kuai
)

# # EXAMPLE_VERSION_INFO is defined by setup.py and passed into the C++ code as a
# # define (VERSION_INFO) here.
# target_compile_definitions(cmake_example
#                            PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})

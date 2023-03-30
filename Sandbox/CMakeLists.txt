cmake_minimum_required(VERSION 3.16)

project(Sandbox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(${PROJECT_NAME}
   src/Main.cpp
)

if (WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        KU_PLATFORM_WINDOWS
    )
else()

endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../kuai ${CMAKE_CURRENT_BINARY_DIR}/kuai)

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

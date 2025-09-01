cmake_minimum_required(VERSION 3.16)

project(sandbox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(${PROJECT_NAME}
   src/Main.cpp
)

if (WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        KU_PLATFORM_WINDOWS
    )
elseif (APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        KU_PLATFORM_APPLE
    )
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../rekuai
    ${CMAKE_CURRENT_BINARY_DIR}/rekuai)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>)

target_include_directories(${PROJECT_NAME}
    PUBLIC "${PROJECT_BINARY_DIR}"
    PUBLIC ../rekuai/src
    PUBLIC ../rekuai/vendor
    PUBLIC ../rekuai/vendor/glm
    PUBLIC ../rekuai/vendor/spdlog/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE rekuai
)

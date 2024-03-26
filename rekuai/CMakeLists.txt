cmake_minimum_required(VERSION 3.16)

project(rekuai VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

option(BUILD_SHARED_LIBS OFF)
set(LIBTYPE STATIC) # Sets OpenAL build type to static

add_library(${PROJECT_NAME}
    src/kpch.h
    src/kpch.cpp
    src/kuai.h

    src/rekuai/Core/App.h
    src/rekuai/Core/App.cpp
    src/rekuai/Core/Core.h
    src/rekuai/Core/EntryPoint.h
    src/rekuai/Core/Input.h
    src/rekuai/Core/KeyCodes.h
    src/rekuai/Core/Log.h
    src/rekuai/Core/Log.cpp
    src/rekuai/Core/MouseBtnCodes.h
    src/rekuai/Core/Timer.h
    src/rekuai/Core/Window.h

    src/rekuai/Events/AppEvent.h
    src/rekuai/Events/Event.h
    src/rekuai/Events/KeyEvent.h
    src/rekuai/Events/MouseEvent.h

    src/rekuai/platform/windows/wininput.cpp
    src/rekuai/platform/windows/winwindow.h
    src/rekuai/platform/windows/winwindow.cpp
    
    src/rekuai/Renderer/Renderer.h 
    src/rekuai/Renderer/Renderer.cpp 
    
    src/rekuai/Components/Components.h
    src/rekuai/Components/EntityManager.h
    src/rekuai/Components/ComponentManager.h
    src/rekuai/Components/ECS.h
 "src/rekuai/Components/System.h" "src/rekuai/Components/SystemManager.h"  "src/rekuai/Components/Entity.h")

set(MY_LIBS
    glfw
    glad
    opengl32
)

if (WIN32)
    # Add dwmapi and windows multimedia for audio
    set(MY_LIBS ${MY_LIBS} dwmapi winmm)

    target_compile_definitions(${PROJECT_NAME} PUBLIC
        KU_PLATFORM_WINDOWS
        WIN32_LEAN_AND_MEAN
        NOMINMAX
        GLFW_INCLUDE_NONE
        _CRT_SECURE_NO_WARNINGS
    )
else()

endif()

target_precompile_headers(${PROJECT_NAME}
    PUBLIC src/kpch.h
)

add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glad)
add_subdirectory(vendor/glm)

target_include_directories(${PROJECT_NAME}
    PUBLIC "${PROJECT_BINARY_DIR}"
    PUBLIC src
    PUBLIC vendor/GLFW/include
    PUBLIC vendor/Glad/include
    PUBLIC vendor/glm
    PUBLIC vendor/spdlog/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${MY_LIBS})

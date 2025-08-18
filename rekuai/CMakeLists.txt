cmake_minimum_required(VERSION 3.16)

project(rekuai VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

    src/rekuai/platform/Windows/wininput.cpp
    src/rekuai/platform/Windows/winwindow.h
    src/rekuai/platform/Windows/winwindow.cpp
    src/rekuai/platform/Vulkan/VulkanAPI.h
    src/rekuai/platform/Vulkan/VulkanAPI.cpp

    src/rekuai/Renderer/Renderer.h
    src/rekuai/Renderer/Renderer.cpp
    src/rekuai/Renderer/RendererAPI.h
    src/rekuai/Renderer/RendererAPI.cpp
    src/rekuai/Renderer/Mesh.h
    src/rekuai/Renderer/Mesh.cpp
    src/rekuai/Renderer/Buffer.h
    src/rekuai/Renderer/Buffer.cpp
    src/rekuai/Renderer/Shader.h
    src/rekuai/Renderer/Shader.cpp
    src/rekuai/Renderer/Framebuffer.h
    src/rekuai/Renderer/Framebuffer.cpp

    src/rekuai/Components/Components.h
    src/rekuai/Components/EntityManager.h
    src/rekuai/Components/ComponentManager.h
    src/rekuai/Components/ECS.h
    src/rekuai/Components/System.h
    src/rekuai/Components/SystemManager.h
    src/rekuai/Components/Entity.h

    src/rekuai/Events/Signal.h
    src/rekuai/Components/Scene.h
    src/rekuai/Components/Scene.cpp
    src/rekuai/Renderer/Material.h
    src/rekuai/Renderer/TextureArray.h
    src/rekuai/Renderer/Texture.h
    src/rekuai/Renderer/Texture.cpp

    vendor/tinyobjloader/tiny_obj_loader.h
    vendor/tinyobjloader/tiny_obj_loader.cpp
    vendor/stb_image/stb_image.h
    vendor/stb_image/stb_image.cpp

    src/rekuai/Renderer/TextureArray.cpp
    vendor/stb_image/stb_image_resize.h
    vendor/stb_image/stb_image_resize.cpp
    src/rekuai/Renderer/TextureManager.h
    src/rekuai/Renderer/TextureManager.cpp
)

set(MY_LIBS
    glfw
    glad
)

if (WIN32)
    # Add dwmapi and windows multimedia for audio
    set(MY_LIBS ${MY_LIBS} opengl32 dwmapi winmm)

    target_compile_definitions(${PROJECT_NAME} PUBLIC
        KU_PLATFORM_WINDOWS
        WIN32_LEAN_AND_MEAN
        NOMINMAX
        GLFW_INCLUDE_NONE
        _CRT_SECURE_NO_WARNINGS
    )
elseif(APPLE)
    # Find Vulkan
    find_package(Vulkan REQUIRED)

    if(NOT Vulkan_FOUND)
        message(FATAL_ERROR "Could not find Vulkan")
    endif()

    # Link Vulkan
    target_include_directories(${PROJECT_NAME} PUBLIC vendor/vulkan)

    # Link the libraries to the target
    set(MY_LIBS
        ${MY_LIBS}
        Vulkan::Vulkan
    )

    target_compile_definitions(${PROJECT_NAME} PUBLIC
        KU_PLATFORM_APPLE
        GLFW_INCLUDE_NONE
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
    PUBLIC vendor/tinyobjloader
    PUBLIC vendor/stb_image
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${MY_LIBS})

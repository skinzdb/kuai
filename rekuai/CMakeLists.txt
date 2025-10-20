cmake_minimum_required(VERSION 3.16)

project(rekuai VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MY_FILES
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

    src/rekuai/Platform/Windows/WinInput.cpp
    src/rekuai/Platform/Windows/WinWindow.h
    src/rekuai/Platform/Windows/WinWindow.cpp

    src/rekuai/Renderer/Renderer.h
    src/rekuai/Renderer/Renderer.cpp
    src/rekuai/Renderer/RendererAPI.h
    src/rekuai/Renderer/RendererAPI.cpp
    src/rekuai/Renderer/Material.h
    src/rekuai/Renderer/Mesh.h
    src/rekuai/Renderer/Mesh.cpp
    src/rekuai/Renderer/Buffer.h
    src/rekuai/Renderer/Buffer.cpp
    src/rekuai/Renderer/Shader.h
    src/rekuai/Renderer/Shader.cpp
    src/rekuai/Renderer/Framebuffer.h
    src/rekuai/Renderer/Framebuffer.cpp
    src/rekuai/Renderer/TextureArray.h
    src/rekuai/Renderer/TextureArray.cpp
    src/rekuai/Renderer/Texture.h
    src/rekuai/Renderer/Texture.cpp

    src/rekuai/Components/Components.h
    src/rekuai/Components/EntityManager.h
    src/rekuai/Components/ComponentManager.h
    src/rekuai/Components/EntityComponentSystem.h
    src/rekuai/Components/System.h
    src/rekuai/Components/SystemManager.h
    src/rekuai/Components/Entity.h

    src/rekuai/Events/Signal.h

    vendor/tinyobjloader/tiny_obj_loader.h
    vendor/tinyobjloader/tiny_obj_loader.cpp
    vendor/stb_image/stb_image.h
    vendor/stb_image/stb_image.cpp

    vendor/stb_image/stb_image_resize.h
    vendor/stb_image/stb_image_resize.cpp
)

set(MY_LIBS
    glfw
    glad
)

set(MY_DEFS GLFW_INCLUDE_NONE)

find_package(Vulkan)

if(NOT APPLE AND Vulkan_FOUND)
    add_library(${PROJECT_NAME} 
        ${MY_FILES}
        src/rekuai/Platform/Vulkan/VulkanAPI.h
        src/rekuai/Platform/Vulkan/VulkanAPI.cpp
        src/rekuai/Platform/Vulkan/VulkanBuffer.h
        src/rekuai/Platform/Vulkan/VulkanBuffer.cpp
        src/rekuai/Platform/Vulkan/VulkanCommand.h
        src/rekuai/Platform/Vulkan/VulkanCommand.cpp
        src/rekuai/Platform/Vulkan/VulkanContext.h
        src/rekuai/Platform/Vulkan/VulkanContext.cpp
        src/rekuai/Platform/Vulkan/VulkanFrame.h
        src/rekuai/Platform/Vulkan/VulkanFrame.cpp
        src/rekuai/Platform/Vulkan/VulkanShader.h
        src/rekuai/Platform/Vulkan/VulkanShader.cpp
        src/rekuai/Platform/Vulkan/VulkanSwapChain.h
        src/rekuai/Platform/Vulkan/VulkanSwapChain.cpp
        src/rekuai/Platform/Vulkan/VulkanUtils.h
    )

    # Link the libraries to the target
    set(MY_LIBS
        ${MY_LIBS}
        Vulkan::Vulkan
    )

    set(MY_DEFS ${MY_DEFS}
        KU_VULKAN
    )
else()
    add_library(${PROJECT_NAME}
        ${MY_FILES}
        src/rekuai/Platform/OpenGL/OpenGLAPI.h
        src/rekuai/Platform/OpenGL/OpenGLAPI.cpp
        src/rekuai/Platform/OpenGL/OpenGLBuffer.h
        src/rekuai/Platform/OpenGL/OpenGLBuffer.cpp
        src/rekuai/Platform/OpenGL/OpenGLShader.h
        src/rekuai/Platform/OpenGL/OpenGLShader.cpp
        src/rekuai/Platform/OpenGL/OpenGLTexture.h
        src/rekuai/Platform/OpenGL/OpenGLTexture.cpp
        src/rekuai/Platform/OpenGL/OpenGLTextureArray.h
        src/rekuai/Platform/OpenGL/OpenGLTextureArray.cpp
    )   

    set(MY_DEFS ${MY_DEFS} KU_OPENGL)
endif()

if (WIN32)
    # Add dwmapi and windows multimedia for audio
    set(MY_LIBS ${MY_LIBS} opengl32 dwmapi winmm)

    set(MY_DEFS ${MY_DEFS}
        KU_PLATFORM_WINDOWS
        WIN32_LEAN_AND_MEAN
        NOMINMAX
        _CRT_SECURE_NO_WARNINGS
    )
elseif(APPLE)
    set(MY_DEFS ${MY_DEFS} KU_PLATFORM_APPLE)
elseif(UNIX)
    set(MY_DEFS ${MY_DEFS}
        KU_PLATFORM_LINUX
    )
endif()

message(STATUS "${MY_DEFS}")

target_compile_definitions(${PROJECT_NAME} PUBLIC ${MY_DEFS})

target_precompile_headers(${PROJECT_NAME}
    PUBLIC src/kpch.h
)

add_subdirectory(vendor/GLFW)
add_subdirectory(vendor/Glad)
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

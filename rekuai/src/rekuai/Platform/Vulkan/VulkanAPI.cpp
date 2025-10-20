#include "kpch.h"

#include "VulkanAPI.h"

#include "rekuai/Core/App.h"
#include "rekuai/Core/Log.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace kuai {
    void VulkanAPI::init()
    {
        // Create Vulkan instance
        context = std::make_unique<VulkanContext>("Hello World!", "kuai");

        // Make window surface
        GLFWwindow *window = reinterpret_cast<GLFWwindow*>(App::get().get_window().get_native_window());
        VkResult result = glfwCreateWindowSurface(context->instance, window, nullptr, &surface);
        if (result != VK_SUCCESS)
        {
            KU_CORE_CRITICAL("(Vulkan) Failed to create window surface, {}", result);
            exit(1);
        }

        // Device selection and creation
        context->pick_physical_device(surface);
        context->create_logical_device(surface);

        // Swap chain, render pass and framebuffer creation
        swap_chain = std::make_unique<VulkanSwapChain>(
            context->device, 
            context->physical_device, 
            surface
        );

        // Command pool
        command = std::make_unique<VulkanCommand>(
            context->device, 
            context->physical_device, 
            surface, 
            swap_chain->get_size()
        );
    }

    void VulkanAPI::stop()
    {
        vkDeviceWaitIdle(context->device);
    }

    void VulkanAPI::clear()
    {

    }

    void VulkanAPI::set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {

    }

    void VulkanAPI::draw_indexed(std::shared_ptr<Shader> shader, std::shared_ptr<VertexArray> vertex_array)
    {
        swap_chain->draw_frame(
            context->device,
            context->physical_device,
            context->graphics_queue,
            context->present_queue,
            surface,
            command->get_command_buffers(),
            std::static_pointer_cast<VulkanShader>(shader),
            std::static_pointer_cast<VulkanVertexArray>(vertex_array)
        );
    }

    VulkanAPI::~VulkanAPI()
    {
        command->cleanup(context->device);
        
        swap_chain->cleanup(context->device);

        context->cleanup_devices();

        vkDestroySurfaceKHR(context->instance, surface, nullptr);

        context->cleanup_instance();
    }

}

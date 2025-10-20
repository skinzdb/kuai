#pragma once

#include "rekuai/Platform/Vulkan/VulkanCommand.h"
#include "rekuai/Platform/Vulkan/VulkanContext.h"
#include "rekuai/Platform/Vulkan/VulkanSwapChain.h"
#include "rekuai/Renderer/RendererAPI.h"
#include "vulkan/vulkan_core.h"

namespace kuai {

    class VulkanAPI : public RendererAPI
    {
    public:
        virtual void init() override;
        virtual void stop() override;
        virtual void clear() override;

        virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

        virtual void draw_indexed(std::shared_ptr<Shader> shader, std::shared_ptr<VertexArray> vertex_array) override;

        ~VulkanAPI();

        VkDevice get_device() const { return context->device; }
        VkPhysicalDevice get_physical_device() const { return context->physical_device; }
        VkQueue get_graphics_queue() const { return context->graphics_queue; }
        VkCommandPool get_command_pool() const { return command->pool; }

    private:
        std::unique_ptr<VulkanContext> context;

        VkSurfaceKHR surface;

        std::unique_ptr<VulkanSwapChain> swap_chain;

        std::unique_ptr<VulkanCommand> command;
    };

}

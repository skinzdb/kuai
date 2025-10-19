#pragma once

#include "rekuai/Platform/Vulkan/VulkanCommand.h"
#include "rekuai/Platform/Vulkan/VulkanContext.h"
#include "rekuai/Platform/Vulkan/VulkanRenderPass.h"
#include "rekuai/Platform/Vulkan/VulkanSwapChain.h"
#include "rekuai/Renderer/Buffer.h"
#include "rekuai/Renderer/RendererAPI.h"
#include "vulkan/vulkan_core.h"

#include <functional>

namespace kuai {

    class VulkanAPI : public RendererAPI
    {
    public:
        virtual void init() override;
        virtual void stop() override;
        virtual void clear() override;

        virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

        virtual void draw_indexed(uint32_t index_count) override;

        ~VulkanAPI();

        VkDevice get_device() const { return context->device; }
        VkPhysicalDevice get_physical_device() const { return context->physical_device; }
        VkQueue get_graphics_queue() const { return context->graphics_queue; }
        VkCommandPool get_command_pool() const { return command->pool; }

        void set_shader_bind_fn(const std::function<void(VkCommandBuffer)>& fn) { command->set_shader_bind_fn(fn); }
        void set_vertex_array_bind_fn(const std::function<void(VkCommandBuffer)>& fn) { command->set_vertex_array_bind_fn(fn); }

    private:
        void create_sync_objects();

    private:
        std::shared_ptr<VulkanContext> context;

        VkSurfaceKHR surface;

        std::shared_ptr<VulkanSwapChain> swap_chain;

        std::shared_ptr<VulkanRenderPass> render_pass;

        std::shared_ptr<VulkanCommand> command;

        std::vector<VkSemaphore> img_available_semaphores;
        std::vector<VkSemaphore> render_finished_semaphores;
        std::vector<VkFence> in_flight_fences;

        uint32_t current_frame = 0;
    };

}

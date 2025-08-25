#pragma once

#include <optional>
#include <vulkan/vulkan.h>

#include "rekuai/Renderer/Buffer.h"
#include "rekuai/Renderer/RendererAPI.h"

namespace kuai {

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;

        bool is_ok() {
            return graphics_family.has_value() && present_family.has_value();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> present_modes;
    };

    class VulkanAPI : public RendererAPI {
    public:
        virtual void init() override;
        virtual void clear() override;

        virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

        virtual void draw_indexed(const std::shared_ptr<VertexArray>& vertex_array, uint32_t index_count) override;

        ~VulkanAPI();

        void set_shader_modules(VkShaderModule vert, VkShaderModule frag);

        VkDevice get_device() const { return device; }

    private:
        void create_instance();
        void setup_debug_messenger();
        void create_surface();
        void pick_physical_device();
        void create_logical_device();
        void create_swap_chain();
        void create_image_views();
        void create_render_pass();
        void create_graphics_pipeline();
        void create_framebuffers();
        void create_command_pool();
        void create_command_buffer();
        void record_command_buffer(VkCommandBuffer buf, uint32_t image_idx);
        void create_sync_objects();
        bool is_device_suitable(const VkPhysicalDevice& device);
        QueueFamilyIndices find_queue_families(const VkPhysicalDevice& device);
        SwapChainSupportDetails query_swap_chain_support(const VkPhysicalDevice& device);

    private:
        VkInstance instance;
        VkDebugUtilsMessengerEXT debug_messenger;
        VkSurfaceKHR surface;
        VkPhysicalDevice physical_device = VK_NULL_HANDLE;
        VkDevice device; // logical device

        VkQueue graphics_queue;
        VkQueue present_queue;

        VkSwapchainKHR swap_chain;
        std::vector<VkImage> swap_chain_images;
        VkFormat swap_chain_image_format;
        VkExtent2D swap_chain_extent;
        std::vector<VkImageView> swap_chain_image_views;

        VkPipelineShaderStageCreateInfo shader_stages[2];
        VkRenderPass render_pass;
        VkPipelineLayout pipeline_layout;
        VkPipeline graphics_pipeline;

        std::vector<VkFramebuffer> swap_chain_framebuffers;

        VkCommandPool command_pool;
        VkCommandBuffer command_buf;

        VkSemaphore img_available_semaphore;
        VkSemaphore render_finished_semaphore;
        VkFence in_flight_fence;

    };

}

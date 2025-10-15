#pragma once

#include "vulkan/vulkan_core.h"

namespace kuai {
    /** \class VulkanContext
	*	\brief Holds instance, physical device, device and queues which will be used by all other Vulkan classes
	*/
    class VulkanContext
    {
    public:
        VulkanContext(const std::string& app_name, const std::string& engine_name);

        void pick_physical_device(VkSurfaceKHR surface);
        void create_logical_device(VkSurfaceKHR surface);
        void cleanup_devices();
        void cleanup_instance();

    private:
        void create_instance();

        void setup_debug_messenger();

    private:
        std::string app_name;
        std::string engine_name;

        VkInstance instance;

        VkDebugUtilsMessengerEXT debug_messenger;

        VkPhysicalDevice physical_device = VK_NULL_HANDLE;
        VkDevice device;

        VkQueue graphics_queue;
        VkQueue present_queue;

        friend class VulkanAPI;
    };
}

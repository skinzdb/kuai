#include "VulkanSwapChain.h"

#include "GLFW/glfw3.h"

#include "rekuai/Core/App.h"
#include "rekuai/Platform/Vulkan/VulkanUtils.h"

namespace kuai {

    void VulkanSwapChain::recreate(VkDevice device, VkPhysicalDevice physical_device, VkRenderPass render_pass)
    {
        vkDeviceWaitIdle(device);

        cleanup(device);

        create(device, physical_device);
        create_image_views(device);
        create_framebuffers(device, render_pass);
    }

    void VulkanSwapChain::cleanup(VkDevice device)
    {
        for (auto framebuffer : swap_chain_framebuffers)
        {
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }

        for (auto image_view : swap_chain_image_views)
        {
            vkDestroyImageView(device, image_view, nullptr);
        }

        vkDestroySwapchainKHR(device, swap_chain, nullptr);
    }

    void VulkanSwapChain::cleanup_surface(VkInstance instance)
    {
        vkDestroySurfaceKHR(instance, surface, nullptr);
    }

    VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats)
    {
        for (const auto& availableFormat : available_formats)
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }

        return available_formats[0];
    }

    VkPresentModeKHR choose_swap_present_mode(const std::vector<VkPresentModeKHR>& available_present_modes)
    {
        for (const auto& availablePresentMode : available_present_modes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        }
        else
        {
            GLFWwindow *window = reinterpret_cast<GLFWwindow*>(App::get().get_window().get_native_window());
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    void VulkanSwapChain::create_surface(VkInstance instance)
    {
        GLFWwindow *window = reinterpret_cast<GLFWwindow*>(App::get().get_window().get_native_window());

        if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
        {
            KU_CORE_CRITICAL("(Vulkan) Failed to create window surface");
            exit(1);
        }
    }

    void VulkanSwapChain::create(VkDevice device, VkPhysicalDevice physical_device)
    {
        SwapChainSupportDetails support_details = VulkanUtils::query_swap_chain_support(physical_device, surface);

        VkSurfaceFormatKHR surfaceFormat = choose_swap_surface_format(support_details.formats);
        VkPresentModeKHR presentMode = choose_swap_present_mode(support_details.present_modes);
        VkExtent2D extent = choose_swap_extent(support_details.capabilities);

        uint32_t imageCount = support_details.capabilities.minImageCount + 1;
        if (support_details.capabilities.maxImageCount > 0 && imageCount > support_details.capabilities.maxImageCount)
        {
            imageCount = support_details.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = VulkanUtils::find_queue_families(physical_device, surface);
        uint32_t queueFamilyIndices[] = {indices.graphics_family.value(), indices.present_family.value()};

        if (indices.graphics_family != indices.present_family) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }

        createInfo.preTransform = support_details.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swap_chain) != VK_SUCCESS)
        {
            KU_CORE_CRITICAL("(Vulkan) Failed to create swap chain");
            exit(1);
        }

        vkGetSwapchainImagesKHR(device, swap_chain, &imageCount, nullptr);
        swap_chain_images.resize(imageCount);
        vkGetSwapchainImagesKHR(device, swap_chain, &imageCount, swap_chain_images.data());

        swap_chain_image_format = surfaceFormat.format;
        swap_chain_extent = extent;
    }

    void VulkanSwapChain::create_image_views(VkDevice device) {
        swap_chain_image_views.resize(swap_chain_images.size());

        for (size_t i = 0; i < swap_chain_images.size(); i++)
        {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = swap_chain_images[i];

            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = swap_chain_image_format;

            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(device, &createInfo, nullptr, &swap_chain_image_views[i]) != VK_SUCCESS)
            {
                KU_CORE_CRITICAL("(Vulkan) Failed to create image views");
                exit(1);
            }
        }
    }

    void VulkanSwapChain::create_framebuffers(VkDevice device, VkRenderPass render_pass)
    {
        swap_chain_framebuffers.resize(swap_chain_image_views.size());

        for (size_t i = 0; i < swap_chain_image_views.size(); i++) {
            VkImageView attachments[] = {
                swap_chain_image_views[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = render_pass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = swap_chain_extent.width;
            framebufferInfo.height = swap_chain_extent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swap_chain_framebuffers[i]) != VK_SUCCESS) {
                KU_CORE_CRITICAL("(Vulkan) Failed to create framebuffer for swap chain");
                exit(1);
            }
        }
    }
}

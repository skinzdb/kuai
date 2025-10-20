#include "VulkanSwapChain.h"

#include "GLFW/glfw3.h"

#include "VulkanFrame.h"
#include "rekuai/Core/App.h"
#include "rekuai/Platform/Vulkan/VulkanUtils.h"

namespace kuai {

    VkSurfaceFormatKHR choose_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats)
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

    VkPresentModeKHR choose_present_mode(const std::vector<VkPresentModeKHR>& available_present_modes)
    {
        for (const auto& availablePresentMode : available_present_modes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D choose_extent(const VkSurfaceCapabilitiesKHR& capabilities)
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

    VulkanSwapChain::VulkanSwapChain(VkDevice device, VkPhysicalDevice physical_device, VkSurfaceKHR surface)
    {
        create(device, physical_device, surface);
        create_sync_objects(device);
    }   

    void VulkanSwapChain::create(VkDevice device, VkPhysicalDevice physical_device, VkSurfaceKHR surface)
    {
        SwapChainSupportDetails support_details = VulkanUtils::query_swap_chain_support(physical_device, surface);

        VkSurfaceFormatKHR surfaceFormat = choose_surface_format(support_details.formats);
        VkPresentModeKHR presentMode = choose_present_mode(support_details.present_modes);
        VkExtent2D chosen_extent = choose_extent(support_details.capabilities);

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
        createInfo.imageExtent = chosen_extent;
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

        if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &chain) != VK_SUCCESS)
        {
            KU_CORE_CRITICAL("(Vulkan) Failed to create swap chain");
            exit(1);
        }

        vkGetSwapchainImagesKHR(device, chain, &imageCount, nullptr);
        images.resize(imageCount);
        vkGetSwapchainImagesKHR(device, chain, &imageCount, images.data());

        image_format = surfaceFormat.format;
        extent = chosen_extent;

        // Create frames
        for (size_t i = 0; i < images.size(); i++) 
        {
            frames.push_back(std::make_unique<VulkanFrame>(device, images[i], image_format));
        }
    }

    void VulkanSwapChain::recreate(VkDevice device, VkPhysicalDevice physical_device, VkSurfaceKHR surface)
    {
        vkDeviceWaitIdle(device);
        
        frames.clear(); // Destroy image views in frames first

        vkDestroySwapchainKHR(device, chain, nullptr);

        create(device, physical_device, surface);
    }

    void VulkanSwapChain::draw_frame(VkDevice device, VkPhysicalDevice physical_device, 
            VkQueue graphics_queue, VkQueue present_queue, 
            VkSurfaceKHR surface, const std::vector<VkCommandBuffer>& cmd_bufs,
            std::shared_ptr<VulkanShader> shader, std::shared_ptr<VulkanVertexArray> vertex_array)
    {
        vkWaitForFences(device, 1, &in_flight_fences[frame_idx], VK_TRUE, UINT64_MAX);

        uint32_t img_idx;
        VkResult result = vkAcquireNextImageKHR(device, chain, UINT64_MAX,
            img_available_semaphores[frame_idx], VK_NULL_HANDLE, &img_idx);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            recreate(device, physical_device, surface);
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            KU_CORE_ERROR("(Vulkan) Failed to acquire swap chain image");
        }

        vkResetFences(device, 1, &in_flight_fences[frame_idx]);

        frames[frame_idx]->record(device, cmd_bufs[frame_idx], extent, 6, shader, vertex_array);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {img_available_semaphores[frame_idx]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &cmd_bufs[frame_idx];

        VkSemaphore signalSemaphores[] = {render_finished_semaphores[img_idx]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(graphics_queue, 1, &submitInfo, in_flight_fences[frame_idx]) != VK_SUCCESS) {
            KU_CORE_ERROR("(Vulkan) Failed to submit draw command buffer");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {chain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &img_idx;

        presentInfo.pResults = nullptr; // Optional

        result = vkQueuePresentKHR(present_queue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        {
            recreate(device, physical_device, surface);
        }
        else if (result != VK_SUCCESS)
        {
            KU_CORE_ERROR("(Vulkan) Failed to submit draw command buffer");
        }

        frame_idx = (frame_idx + 1) % images.size();
    }

    void VulkanSwapChain::cleanup(VkDevice device)
    {
        for (size_t i = 0; i < images.size(); i++) 
        {
            vkDestroySemaphore(device, render_finished_semaphores[i], nullptr);
            vkDestroySemaphore(device, img_available_semaphores[i], nullptr);
            vkDestroyFence(device, in_flight_fences[i], nullptr);
        }

        frames.clear();

        vkDestroySwapchainKHR(device, chain, nullptr);
    }

    void VulkanSwapChain::create_sync_objects(VkDevice device)
    {
        img_available_semaphores.resize(images.size());
        render_finished_semaphores.resize(images.size());
        in_flight_fences.resize(images.size());

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < images.size(); i++)
        {
            if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &img_available_semaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(device, &semaphoreInfo, nullptr, &render_finished_semaphores[i]) != VK_SUCCESS ||
                vkCreateFence(device, &fenceInfo, nullptr, &in_flight_fences[i]) != VK_SUCCESS)
            {
                KU_CORE_CRITICAL("(Vulkan) Failed to create synchronisation primitives");
                exit(1);
            }
        }
    }
}

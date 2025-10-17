#include "VulkanAPI.h"

#include "rekuai/Core/Log.h"
#include "rekuai/Core/Util.h"
#include "rekuai/Platform/Vulkan/VulkanBuffer.h"
#include "rekuai/Platform/Vulkan/VulkanCommand.h"
#include "rekuai/Platform/Vulkan/VulkanContext.h"
#include "rekuai/Platform/Vulkan/VulkanPipeline.h"
#include "rekuai/Platform/Vulkan/VulkanRenderPass.h"
#include "rekuai/Platform/Vulkan/VulkanShader.h"
#include "rekuai/Platform/Vulkan/VulkanSwapChain.h"
#include "rekuai/Renderer/Buffer.h"
#include "vulkan/vulkan_core.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace kuai {
    void VulkanAPI::init()
    {
        context = std::make_shared<VulkanContext>("Hello World!", "kuai");

        // Make window surface
        swap_chain = std::make_shared<VulkanSwapChain>();
        swap_chain->create_surface(context->instance);

        // Device selection and creation
        context->pick_physical_device(swap_chain->surface);
        context->create_logical_device(swap_chain->surface);

        // Swap chain, render pass and framebuffer creation
        swap_chain->create(context->device, context->physical_device);
        swap_chain->create_image_views(context->device);
        render_pass = std::make_shared<VulkanRenderPass>(context->device, swap_chain->swap_chain_image_format);
        swap_chain->create_framebuffers(context->device, render_pass->render_pass);

        // In-built shader
        std::string vert_src = read_file("vert.spv");
        std::string frag_src = read_file("frag.spv");
        shader = std::make_shared<VulkanShader>(context->device, vert_src, frag_src);

        // Graphics pipeline
        pipeline = std::make_shared<VulkanPipeline>();
        pipeline->set_shaders(shader->vert, shader->frag);
        pipeline->set_vertex_layout(BufferLayout {
            { ShaderDataType::VEC3, "positions" },
            { ShaderDataType::VEC3, "normals" },
            { ShaderDataType::VEC2, "tex_coords" }
        });
        pipeline->create(context->device, render_pass->render_pass);
        vkDestroyShaderModule(context->device, shader->vert, nullptr);
        vkDestroyShaderModule(context->device, shader->frag, nullptr);

        command = std::make_shared<VulkanCommand>(context->device, context->physical_device, swap_chain);

        create_sync_objects();
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

    void VulkanAPI::draw_indexed(std::shared_ptr<VertexArray> vertex_array, uint32_t index_count)
    {
        vkWaitForFences(context->device, 1, &in_flight_fences[current_frame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(context->device, swap_chain->swap_chain, UINT64_MAX,
            img_available_semaphores[current_frame], VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            swap_chain->recreate(context->device, context->physical_device, render_pass->render_pass);
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            KU_CORE_ERROR("(Vulkan) Failed to acquire swap chain image");
        }

        vkResetFences(context->device, 1, &in_flight_fences[current_frame]);

        vkResetCommandBuffer(command->command_bufs[current_frame], 0);
        command->record(context->device, swap_chain, pipeline, render_pass->render_pass,
            command->command_bufs[current_frame], imageIndex, vertex_array);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {img_available_semaphores[current_frame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &command->command_bufs[current_frame];

        VkSemaphore signalSemaphores[] = {render_finished_semaphores[current_frame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(context->graphics_queue, 1, &submitInfo, in_flight_fences[current_frame]) != VK_SUCCESS) {
            KU_CORE_ERROR("(Vulkan) Failed to submit draw command buffer");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {swap_chain->swap_chain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;

        presentInfo.pResults = nullptr; // Optional

        result = vkQueuePresentKHR(context->present_queue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        {
            swap_chain->recreate(context->device, context->physical_device, render_pass->render_pass);
        }
        else if (result != VK_SUCCESS)
        {
            KU_CORE_ERROR("(Vulkan) Failed to submit draw command buffer");
        }

        current_frame = (current_frame + 1) % swap_chain->swap_chain_images.size();
    }

    void VulkanAPI::create_sync_objects()
    {
        img_available_semaphores.resize(swap_chain->swap_chain_images.size());
        render_finished_semaphores.resize(swap_chain->swap_chain_images.size());
        in_flight_fences.resize(swap_chain->swap_chain_images.size());

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < swap_chain->swap_chain_images.size(); i++)
        {
            if (vkCreateSemaphore(context->device, &semaphoreInfo, nullptr, &img_available_semaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(context->device, &semaphoreInfo, nullptr, &render_finished_semaphores[i]) != VK_SUCCESS ||
                vkCreateFence(context->device, &fenceInfo, nullptr, &in_flight_fences[i]) != VK_SUCCESS)
            {
                KU_CORE_CRITICAL("(Vulkan) Failed to create synchronisation primitives");
                exit(1);
            }
        }
    }

    VulkanAPI::~VulkanAPI()
    {
        swap_chain->cleanup(context->device);

        pipeline->cleanup(context->device);

        render_pass->cleanup(context->device);

        for (size_t i = 0; i < swap_chain->swap_chain_images.size(); i++)
        {
            vkDestroySemaphore(context->device, render_finished_semaphores[i], nullptr);
            vkDestroySemaphore(context->device, img_available_semaphores[i], nullptr);
            vkDestroyFence(context->device, in_flight_fences[i], nullptr);
        }

        command->cleanup(context->device);

        context->cleanup_devices();

        swap_chain->cleanup_surface(context->instance);

        context->cleanup_instance();
    }

}

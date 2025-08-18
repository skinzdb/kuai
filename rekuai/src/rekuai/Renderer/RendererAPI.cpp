
#include "RendererAPI.h"

#include "rekuai/Platform/Vulkan/VulkanAPI.h"

namespace kuai {

    RendererAPI::API RendererAPI::api = RendererAPI::API::Vulkan;

    std::unique_ptr<RendererAPI> RendererAPI::create() {
        #ifdef KU_PLATFORM_WINDOWS
            return nullptr;
        #endif

        #ifdef KU_PLATFORM_APPLE
            return std::make_unique<VulkanAPI>();
        #endif

        return nullptr;
    }
}

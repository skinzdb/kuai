
#include "RendererAPI.h"

#include "rekuai/Platform/Vulkan/VulkanAPI.h"

namespace kuai {

    RendererAPI::API RendererAPI::api = RendererAPI::API::Vulkan;

    std::unique_ptr<RendererAPI> RendererAPI::create() {
        switch (getAPI()) {
            case API::None:
                return nullptr;

            case API::OpenGL:
                return nullptr;

            case API::Vulkan:
                return std::make_unique<VulkanAPI>();
        }
    }
}

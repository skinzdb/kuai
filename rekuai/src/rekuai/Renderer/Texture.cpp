#include "kpch.h"
#include "rekuai/Renderer/RendererAPI.h"
#include "Texture.h"

namespace kuai {
    std::shared_ptr<Texture2D> Texture2D::create(const std::string& filename) {
        switch (RendererAPI::getAPI()) {
            case RendererAPI::API::None:
                return nullptr;

            case RendererAPI::API::OpenGL:
                return nullptr;

            case RendererAPI::API::Vulkan:
                return nullptr;
        }
    }
}

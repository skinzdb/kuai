
#include "RendererAPI.h"

#ifdef KU_VULKAN
    #include "rekuai/Platform/Vulkan/VulkanAPI.h"
#endif

#ifdef KU_OPENGL
    #include "rekuai/Platform/OpenGL/OpenGLAPI.h"
#endif 

namespace kuai {

    RendererAPI::API RendererAPI::api = RendererAPI::API::Vulkan;

    std::unique_ptr<RendererAPI> RendererAPI::create() 
    {
        #ifdef KU_VULKAN
            return std::make_unique<VulkanAPI>();
        #endif
           
        #ifdef KU_OPENGL
           return std::make_unique<OpenGLAPI>();
        #endif 

        return nullptr;
    }
}

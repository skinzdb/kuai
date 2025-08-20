#include "Framebuffer.h"
#include "rekuai/Renderer/RendererAPI.h"

#ifdef KU_VULKAN

#endif 

#ifdef KU_OPENGL
    #include "rekuai/Platform/OpenGL/OpenGLFramebuffer.h"
#endif

namespace kuai {
    std::shared_ptr<Framebuffer> Framebuffer::create(uint32_t width, uint32_t height, uint32_t samples, uint32_t attachments) 
    {
        #ifdef KU_VULKAN
            return nullptr;
        #endif 

        #ifdef KU_OPENGL
            return std::make_unique<OpenGLFramebuffer>(width, height, samples, attachments);
        #endif 
          
        return nullptr;
    }
}

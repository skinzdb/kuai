#include "kpch.h"

#include "TextureArray.h"

#ifdef KU_VULKAN

#endif

#ifdef KU_OPENGL
	#include "rekuai/Platform/OpenGL/OpenGLTextureArray.h"
#endif

namespace kuai {
    std::shared_ptr<TextureArray> TextureArray::create(uint32_t width, uint32_t height, uint32_t layers)
    {
		#ifdef KU_VULKAN

		#endif

		#ifdef KU_OPENGL
        	return std::shared_ptr<OpenGLTextureArray>();
		#endif

		return nullptr;
    }
}

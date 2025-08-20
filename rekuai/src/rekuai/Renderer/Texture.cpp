#include "kpch.h"
#include "Texture.h"

#ifdef KU_VULKAN

#endif

#ifdef KU_OPENGL
	#include "rekuai/Platform/OpenGL/OpenGLTexture.h"
#endif

namespace kuai {
    std::shared_ptr<Texture> Texture::create(const std::string& filename) 
    {
	    #ifdef KU_VULKAN

		#endif

		#ifdef KU_OPENGL
        	return std::make_shared<OpenGLTexture>(filename);
		#endif

		return nullptr;
    }
}

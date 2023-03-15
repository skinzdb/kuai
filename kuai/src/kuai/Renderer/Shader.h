#pragma once

#include "ShaderProgram.h"

namespace kuai {
	// Forward declaration
	class Entity;

	const uint32_t MAX_LIGHTS = 10;

	class Shader : public ShaderProgram
	{
	public:
		Shader(const std::string& vertSrc, const std::string& fragSrc);
		~Shader();

		virtual void update() = 0;

	private:
		void setLight(Entity* light);

		friend class RenderSystem;
	};
}



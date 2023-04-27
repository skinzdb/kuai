#pragma once

#include "StaticShader.h"
#include "Texture.h"
#include "Cubemap.h"

#include "glm/glm.hpp"
#include <memory>

namespace kuai {
	/** \class Material 
	*   \brief Base class for materials. Every material has a shader that sets rendering properties in each render loop.
	*/
	class Material
	{
	public:
		virtual void render() = 0;

		void setShader(Shader* shader) { this->shader = shader; };
		Shader* getShader() { return shader; }

	protected:
		Shader* shader = nullptr;
	};

	class DefaultMaterial : public Material
	{
	public:
		DefaultMaterial(Rc<Texture> diffuse, Rc<Texture> specular, float specularAmount)
			: diffuse(diffuse), specular(specular), specularAmount(specularAmount), reflections(false)
		{
			shader = StaticShader::basic;
		}

		virtual void render()
		{
			diffuse->bind(0);
			specular->bind(1);
			if (reflectionMap)
				reflectionMap->bind(4);
		}

		void setDiffuse(Rc<Texture> diffuse) { this->diffuse = diffuse; }
		void setSpecular(Rc<Texture> specular) { this->specular = specular; }
		void setReflection(Rc<Cubemap> reflection) { this->reflectionMap = reflection; reflections = true; }

		void setTiling(float x, float y) { tilingFactor = glm::vec2(x, y); }

		float specularAmount;
		bool reflections;

	private:
		Rc<Texture> diffuse;
		Rc<Texture> specular;
		Rc<Cubemap> reflectionMap;

		glm::vec2 tilingFactor = { 1.0f, 1.0f };

		friend class Model;
	};

	class SkyboxMaterial : public Material
	{
	public:
		SkyboxMaterial(Rc<Cubemap> cubemap) : cubemap(cubemap)
		{
			shader = StaticShader::skybox;
		}

		virtual void render()
		{
			cubemap->bind(0);
		}

	private:
		Rc<Cubemap> cubemap;
	};
}
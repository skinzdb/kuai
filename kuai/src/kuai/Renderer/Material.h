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
		Material() { materialId = materialCounter++; }

		virtual void bind() = 0;

		void setShader(Shader* shader) { this->shader = shader; };
		Shader* getShader() { return shader; }

	protected:
		Shader* shader = nullptr;

		u32 materialId;
	private:
		static u32 materialCounter;
	};

	class DefaultMaterial : public Material
	{
	public:
		DefaultMaterial(Rc<Texture> diffuse, Rc<Texture> specular, float specularAmount)
			: diffuse(diffuse), specular(specular), specularAmount(specularAmount), reflections(false)
		{
			shader = StaticShader::basic;
		}

		virtual void bind()
		{
			diffuse->bind(materialId);
			specular->bind(materialId + 1);
			if (reflectionMap)
				reflectionMap->bind(materialId + 2);
		}

		void setDiffuse(Rc<Texture> diffuse) { this->diffuse = diffuse; }
		void setSpecular(Rc<Texture> specular) { this->specular = specular; }
		void setReflection(Rc<Cubemap> reflection) { this->reflectionMap = reflection; reflections = true; }

		float getSpecularAmount() const { return specularAmount; }
		void setSpecularAmount(float amount) { specularAmount = amount; }

		bool hasReflections() const { return reflections; }
		void setReflections(bool reflections) { this->reflections = reflections; }

		glm::vec2 getTiling() const { return tilingFactor; }
		void setTiling(float x, float y) { tilingFactor = glm::vec2(x, y); }

	private:
		Rc<Texture> diffuse;
		Rc<Texture> specular;
		Rc<Cubemap> reflectionMap;

		float specularAmount;
		bool reflections;
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

		virtual void bind()
		{
			cubemap->bind(0);
		}

	private:
		Rc<Cubemap> cubemap;
	};
}
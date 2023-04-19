#pragma once

#include "StaticShader.h"
#include "Texture.h"
#include "Cubemap.h"

#include <glm/glm.hpp>
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
		DefaultMaterial(std::shared_ptr<Texture> diffuse, std::shared_ptr<Texture> specular, float specularAmount)
			: diffuse(diffuse), specular(specular), specularAmount(specularAmount)
		{
			shader = StaticShader::basic;
		}

		virtual void render()
		{
			diffuse->bind(0);
			specular->bind(1);
		}

		void setDiffuse(std::shared_ptr<Texture> diffuse) { this->diffuse = diffuse; }
		void setSpecular(std::shared_ptr<Texture> specular) { this->specular = specular; }

	private:
		std::shared_ptr<Texture> diffuse;
		std::shared_ptr<Texture> specular;

		float specularAmount;

		friend class Model;
	};

	class SkyboxMaterial : public Material
	{
	public:
		SkyboxMaterial(std::shared_ptr<Cubemap> cubemap) : cubemap(cubemap)
		{
			shader = StaticShader::skybox;
		}

		virtual void render()
		{
			cubemap->bind();
		}

	private:
		std::shared_ptr<Cubemap> cubemap;
	};
}
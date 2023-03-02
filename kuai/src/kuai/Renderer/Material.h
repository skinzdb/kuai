#pragma once

#include "Shader.h"
#include "StaticShader.h"
#include "Texture.h"
#include "glm/glm.hpp"

namespace kuai {
	class Material
	{
	public:
		virtual void render() = 0;
		Shader* shader = nullptr;
	};

	class PhongMaterial : public Material
	{
	public:
		PhongMaterial(std::shared_ptr<Texture> diffuse, std::shared_ptr<Texture> specular, float shininess) :
			shininess(shininess), diffuse(diffuse), specular(specular)
		{
			shader = StaticShader::Phong;
		}

		virtual void render()
		{
			StaticShader::Phong->setUniforms(shininess);

			diffuse->bind(0);
			specular->bind(1);
		}

	private:
		std::shared_ptr<Texture> diffuse;
		std::shared_ptr<Texture> specular;

		float shininess = 1.0f;
	};

	class SimpleMaterial : public Material {
	public:
		SimpleMaterial()
		{
			shader = StaticShader::Simple;
		}

		SimpleMaterial(glm::vec3& col) : col(col)
		{
			SimpleMaterial();
		}

		virtual void render() 
		{
			StaticShader::Simple->setUniforms(col);
		}

	private:
		glm::vec3 col = { 1.0f, 1.0f, 1.0f };
	};
}
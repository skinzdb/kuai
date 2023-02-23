#pragma once

#include "Shader.h"
#include "StaticShader.h"
#include "Texture.h"
#include "glm/glm.hpp"

namespace Smong {
	class Material
	{
	public:
		virtual void Render() = 0;
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

		virtual void Render()
		{
			StaticShader::Phong->SetUniforms(shininess);

			diffuse->Bind(0);
			specular->Bind(1);
		}

	private:
		std::shared_ptr<Texture> diffuse;
		std::shared_ptr<Texture> specular;

		float shininess = 1.0f;
	};

	class SimpleMaterial : Material {
	public:
		SimpleMaterial()
		{
			shader = StaticShader::Simple;
		}

		SimpleMaterial(glm::vec3& col) : col(col)
		{
			SimpleMaterial();
		}

		virtual void Render() 
		{
			StaticShader::Simple->SetUniforms(col);
		}

	private:
		glm::vec3 col = { 1.0f, 1.0f, 1.0f };
	};
}
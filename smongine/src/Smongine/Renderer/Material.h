#pragma once

#include "Shader.h"
#include "StaticShader.h"
#include "Texture.h"
#include "glm/glm.hpp"

namespace Smong {
	struct Material
	{
		virtual void Render() = 0;
		Shader* shader = nullptr;
	};

	struct PhongMaterial : Material
	{
		PhongMaterial(Texture& diffuse, Texture& specular, float shininess) :
			shininess(shininess), diffuse(diffuse), specular(specular)
		{
			shader = StaticShader::Phong;
		}

		Texture& diffuse;
		Texture& specular;

		float shininess = 1.0f;

		virtual void Render()
		{
			StaticShader::Phong->SetUniforms(shininess);

			diffuse.Bind(0);
			specular.Bind(1);
		}
	};

	struct SimpleMaterial : Material {
		SimpleMaterial()
		{
			shader = StaticShader::Simple;
		}

		SimpleMaterial(glm::vec3& col) : col(col)
		{
			SimpleMaterial();
		}

		glm::vec3 col = { 1.0f, 1.0f, 1.0f };

		virtual void Render() 
		{
			StaticShader::Simple->SetUniforms(col);
		}
	};
}
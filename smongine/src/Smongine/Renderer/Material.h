#pragma once

#include "Shader.h"
#include "StaticShader.h"
#include "Texture.h"
#include "glm/glm.hpp"

namespace Smong {
	class Material
	{
	protected:
		virtual void Render() = 0;
	};

	class PhongMaterial : public Material
	{
	public:
		PhongMaterial(float shininess, float reflectivity, Texture* diffuse, Texture* normal, Texture* specular, glm::vec3& col) :
			shininess(shininess), reflectivity(reflectivity), diffuse(diffuse), normal(normal), specular(specular), col(col)
		{
		}

		Texture* GetDiffuse() { return diffuse; }
		void SetDiffuse(Texture* diffuse) { this->diffuse = diffuse; }

		Texture* GetNormal() { return normal; }
		void SetNormal(Texture* normal) { this->normal = normal; }

		Texture* GetSpecular() { return specular; }
		void SetSpecular(Texture* specular) { this->specular = specular; }

		glm::vec3 GetCol() { return col; }
		void SetCol(glm::vec3 col) { this->col = col; }

		float GetShininess() { return shininess; }
		void SetShininess(float shininess) { this->shininess = shininess; }

		float GetReflectivity() { return reflectivity; }
		void SetReflectivity(float reflectivity) { this->reflectivity = reflectivity; }

		virtual void Render()
		{
			StaticShader::Phong->SetUniforms(col, shininess, reflectivity);

			if (diffuse)
				diffuse->Bind();
			if (normal)
				normal->Bind();
			if (specular)
				specular->Bind();
		}

	private:
		Texture* diffuse;
		Texture* normal;
		Texture* specular;

		glm::vec3 col;
		float shininess;
		float reflectivity;
	};
}
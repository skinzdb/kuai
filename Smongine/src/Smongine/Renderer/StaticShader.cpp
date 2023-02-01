#include "smpch.h"
#include "StaticShader.h"

namespace Smong {
	PhongShader* StaticShader::Phong = nullptr;
	SimpleShader* StaticShader::Simple = nullptr;

	void StaticShader::Init()
	{
		Phong = new PhongShader();
		Simple = new SimpleShader();
	}

	void StaticShader::Cleanup()
	{
		delete Phong;
		delete Simple;
	}

	// *** PHONG ******************************************************************

	const char* PHONG_VERT_SRC = R"(
		#version 460
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec3 aNormals;
		layout (location = 2) in vec2 aTexCoords;

		out vec4 worldPos;
		out vec3 worldNorm;
		out vec2 texCoords;

		uniform mat4 projectionMatrix;
		uniform mat4 viewMatrix;

		//layout (std140, binding = 0) uniform Matrices
		//{
		//	uniform mat4 projectionMatrix;
		//	uniform mat4 viewMatrix;
		//};

		uniform mat4 modelMatrix;
		uniform mat3 model3x3InvTransp; // Used to calculate proper world position of normals

		void main()
		{
			worldPos = modelMatrix * vec4(aPos, 1.0);
			worldNorm = model3x3InvTransp * aNormals;
			texCoords = aTexCoords;
			gl_Position = projectionMatrix * viewMatrix * worldPos;
		}
	)";

	const char* PHONG_FRAG_SRC = R"(
		#version 460
		out vec4 fragCol;

		in vec4 worldPos;
		in vec3 worldNorm;
		in vec2 texCoords;

		uniform vec3 viewPos;

		#define NUM_LIGHTS 10

		struct Light
		{
			int type;

			vec3 pos;
			vec3 dir;
			vec3 col;
			
			float intensity;

			float linear;
			float quadratic;

			float cutoff;
		};
		uniform Light lights[NUM_LIGHTS];

		struct Material
		{
			sampler2D diffuse;
			sampler2D specular;
			float shininess;
		};
		uniform Material material;

		void main()
		{
			vec3 norm = normalize(worldNorm);
			vec3 viewDir = normalize(viewPos - worldPos.xyz);
			vec3 lightDir = vec3(0.0, 0.0, 0.0);
			float factor = 1.0;

			vec3 finalCol = vec3(0.0, 0.0, 0.0);

			for (int i = 0; i < NUM_LIGHTS; i++)
			{
				if (lights[i].type == 0) // Directional Light
				{
					lightDir = -lights[i].dir;
					factor = 1.0;
				} 
				else
				{
					lightDir = normalize(lights[i].pos - worldPos.xyz);

					// Attenuation
					float distance = length(lights[i].pos - worldPos.xyz);
					factor = 1.0 / (1.0 + lights[i].linear * distance + lights[i].quadratic * (distance * distance));    

					if (lights[i].type == 2) // Spotlight
					{
						float theta = dot(lightDir, normalize(-lights[i].dir)); 
						float epsilon = 0.1;
						float intensity = clamp((theta - lights[i].cutoff) / epsilon, 0.0, 1.0);

						factor *= intensity;
					}
				}

				vec3 lighting = lights[i].intensity * lights[i].col;

				// Ambient
				vec3 ambient = 0.1 * lighting * vec3(texture(material.diffuse, texCoords));

				// Diffuse
				float diff = max(dot(norm, lightDir), 0.0);
				vec3 diffuse = lighting * diff * vec3(texture(material.diffuse, texCoords));

				// Specular
				vec3 reflectDir = reflect(-lightDir, norm);
				vec3 halfwayDir = normalize(lightDir + viewDir);
				float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
				vec3 specular = lighting * spec * vec3(texture(material.specular, texCoords));

				finalCol += (ambient + diffuse + specular) * factor;
			}

			fragCol = vec4(finalCol, 1.0);
		}
	)";

	PhongShader::PhongShader() : Shader(PHONG_VERT_SRC, PHONG_FRAG_SRC)
	{
		Bind();
	
		CreateUniform("projectionMatrix");
		CreateUniform("viewMatrix");

		//std::vector<const char*> matrixMemberNames = { "projectionMatrix", "viewMatrix" };

		//CreateUniformBlock("Matrices", matrixMemberNames, sizeof(glm::mat4) * 2);

		CreateUniform("modelMatrix");
		CreateUniform("model3x3InvTransp");

		CreateUniform("material.diffuse");
		CreateUniform("material.specular");
		CreateUniform("material.shininess");
		SetUniform("material.diffuse", 0);
		SetUniform("material.specular", 1);

		for (int i = 0; i < NUM_LIGHTS; i++)
		{
			CreateUniform("lights[" + std::to_string(i) + "].type");

			CreateUniform("lights[" + std::to_string(i) + "].pos");
			CreateUniform("lights[" + std::to_string(i) + "].dir");
			CreateUniform("lights[" + std::to_string(i) + "].col");

			CreateUniform("lights[" + std::to_string(i) + "].intensity");

			CreateUniform("lights[" + std::to_string(i) + "].linear");
			CreateUniform("lights[" + std::to_string(i) + "].quadratic");

			CreateUniform("lights[" + std::to_string(i) + "].cutoff");
		}

		CreateUniform("viewPos");
	}

	void PhongShader::SetUniforms(float shininess)
	{
		SetUniform("material.shininess", shininess);
	}

	// *** SIMPLE *****************************************************************

	const char* SIMPLE_VERT_SRC = R"(
		#version 330 core
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec3 aNormals;

		out vec4 worldPos;
		out vec3 worldNorm;

		uniform mat4 modelMatrix;
		uniform mat4 viewMatrix;
		uniform mat4 projectionMatrix;

		uniform mat3 model3x3InvTransp; // Used to calculate proper world position of normals

		void main()
		{
			worldPos = modelMatrix * vec4(aPos, 1.0);
			worldNorm = model3x3InvTransp * aNormals;
			gl_Position = projectionMatrix * viewMatrix * worldPos;
		}
	)";

	const char* SIMPLE_FRAG_SRC = R"(
		#version 330 core
		out vec4 FragColor;

		in vec4 worldPos;
		in vec3 worldNorm;

		struct Material 
		{
			vec3 col;
		};
		uniform Material material;

		struct Light
		{
			vec3 pos;
			vec3 col;
			float intensity;
		};
		uniform Light light;

		void main()
		{	
			vec3 lighting = light.col * light.intensity;

			// Ambient
			vec3 ambient = lighting * material.col;

			// Diffuse
			vec3 norm = normalize(worldNorm);
			vec3 lightDir = normalize(light.pos - worldPos.xyz);
			float diff = max(dot(norm, lightDir), 0.0);
			vec3 diffuse = diff * material.col;

			vec3 finalCol = ambient + diffuse;

			FragColor = vec4(finalCol, 1.0);
		}
	)";

	SimpleShader::SimpleShader() : Shader(SIMPLE_VERT_SRC, SIMPLE_FRAG_SRC)
	{
		Bind();

		CreateUniform("modelMatrix");
		CreateUniform("viewMatrix");
		CreateUniform("projectionMatrix");

		CreateUniform("model3x3InvTransp");

		CreateUniform("material.col");

		CreateUniform("light.pos");
		CreateUniform("light.col");
		CreateUniform("light.intensity");
	}

	void SimpleShader::SetUniforms(const glm::vec3& col) const
	{
		SetUniform("material.col", col);
	}
}



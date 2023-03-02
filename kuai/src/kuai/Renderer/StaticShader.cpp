#include "kpch.h"
#include "StaticShader.h"

namespace kuai {
	PhongShader* StaticShader::Phong = nullptr;
	SimpleShader* StaticShader::Simple = nullptr;

	void StaticShader::init()
	{
		Phong = new PhongShader();
		Simple = new SimpleShader();
	}

	void StaticShader::cleanup()
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
		bind();
	
		createUniform("projectionMatrix");
		createUniform("viewMatrix");

		//std::vector<const char*> matrixMemberNames = { "projectionMatrix", "viewMatrix" };

		//createUniformBlock("Matrices", matrixMemberNames, sizeof(glm::mat4) * 2);

		createUniform("modelMatrix");
		createUniform("model3x3InvTransp");

		createUniform("material.diffuse");
		createUniform("material.specular");
		createUniform("material.shininess");
		setUniform("material.diffuse", 0);
		setUniform("material.specular", 1);

		for (int i = 0; i < NUM_LIGHTS; i++)
		{
			createUniform("lights[" + std::to_string(i) + "].type");

			createUniform("lights[" + std::to_string(i) + "].pos");
			createUniform("lights[" + std::to_string(i) + "].dir");
			createUniform("lights[" + std::to_string(i) + "].col");

			createUniform("lights[" + std::to_string(i) + "].intensity");

			createUniform("lights[" + std::to_string(i) + "].linear");
			createUniform("lights[" + std::to_string(i) + "].quadratic");

			createUniform("lights[" + std::to_string(i) + "].cutoff");
		}

		createUniform("viewPos");
	}

	void PhongShader::setUniforms(float shininess)
	{
		setUniform("material.shininess", shininess);
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
			vec3 ambient = vec3(0.1, 0.1, 0.1);

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
		bind();

		createUniform("modelMatrix");
		createUniform("viewMatrix");
		createUniform("projectionMatrix");

		createUniform("model3x3InvTransp");

		createUniform("material.col");

		createUniform("light.pos");
		createUniform("light.col");
		createUniform("light.intensity");
	}

	void SimpleShader::setUniforms(const glm::vec3& col) const
	{
		setUniform("material.col", col);
	}
}



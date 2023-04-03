#include "kpch.h"
#include "StaticShader.h"

#include "kuai/Components/Components.h"

#include "glm/gtc/matrix_inverse.hpp"

namespace kuai {
	IShader* StaticShader::basic = nullptr;
	IShader* StaticShader::skybox = nullptr;
	std::unordered_map<std::string, uint32_t> StaticShader::ubos = std::unordered_map<std::string, uint32_t>();

	IShader::IShader(const std::string& vertSrc, const std::string& fragSrc) : Shader(vertSrc, fragSrc), shaderData(std::make_shared<ShaderData>())
	{
	}

	void IShader::updateTransform()
	{
		bind();

		setUniform("modelMatrix", shaderData->modelMatrix);
		setUniform("model3x3InvTransp", glm::inverseTranspose(glm::mat3(shaderData->modelMatrix)));
	}

	void IShader::updateCamera()
	{
		bind();

		//setUniform(StaticShader::getUboId("Matrices"), &shaderData->projectionMatrix[0][0], sizeof(glm::mat4));
		//setUniform(StaticShader::getUboId("Matrices"), &shaderData->viewMatrix[0][0], sizeof(glm::mat4), sizeof(glm::mat4));
	
		setUniform("projectionMatrix", shaderData->projectionMatrix);
		setUniform("viewMatrix", shaderData->viewMatrix);
	}

	const char* DEFAULT_VERT = R"(
		#version 460
		layout (location = 0) in vec3 pos;
		layout (location = 1) in vec3 normal;
		layout (location = 2) in vec2 texCoord;

		//layout (std140, binding = 0) uniform Matrices
		//{
		//	mat4 projectionMatrix;
		//	mat4 viewMatrix;
		//};

		uniform mat4 projectionMatrix;
		uniform mat4 viewMatrix;		

		uniform mat4 modelMatrix;
		uniform mat3 model3x3InvTransp; // Used to calculate proper world position of normals		

		out vec4 worldPos;
		out vec3 worldNorm;
		out vec2 texCoords;

		void main()
		{
			worldPos = modelMatrix * vec4(pos, 1.0);
			worldNorm = model3x3InvTransp * normal;
			texCoords = texCoord;
			gl_Position = projectionMatrix * viewMatrix * worldPos;
		}
	)";

	const char* DEFAULT_FRAG = R"(
		#version 460
		#define MAX_LIGHTS 10

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

		uniform Light lights[MAX_LIGHTS];
		//layout (shared) uniform Lights
		//{
		//	Light lights[MAX_LIGHTS];
		//};

		out vec4 fragCol;

		in vec4 worldPos;
		in vec3 worldNorm;
		in vec2 texCoords;

		uniform vec3 viewPos;

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

			for (int i = 0; i < MAX_LIGHTS; i++)
			{
				if (lights[i].type == 0) // Directional Light
				{
					lightDir = normalize(-lights[i].dir);
					factor = lights[i].intensity;
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

				// Ambient
				vec3 ambient = 0.1 * lights[i].col * vec3(texture(material.diffuse, texCoords));

				// Diffuse
				float diff = max(dot(norm, lightDir), 0.0);
				vec3 diffuse = lights[i].col * diff * vec3(texture(material.diffuse, texCoords));

				// Specular
				vec3 reflectDir = reflect(-lightDir, norm);
				vec3 halfwayDir = normalize(lightDir + viewDir);
				float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
				vec3 specular = spec * vec3(texture(material.specular, texCoords));

				finalCol += (ambient + diffuse + specular) * factor;
			}

			fragCol = vec4(finalCol, 1.0);
		}
	)";

	DefaultShader::DefaultShader() : IShader(DEFAULT_VERT, DEFAULT_FRAG)
	{
		bind();

		createUniform("projectionMatrix");
		createUniform("viewMatrix");

		createUniform("modelMatrix");
		createUniform("model3x3InvTransp");

		for (int i = 0; i < MAX_LIGHTS; i++)
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

		createUniform("material.diffuse");
		createUniform("material.specular");
		createUniform("material.shininess");
		setUniform("material.diffuse", 0);
		setUniform("material.specular", 1);

		setUniform("material.shininess", 35.0f);

		createUniform("viewPos");
	}

	void DefaultShader::updateLight()
	{
		Light* l = shaderData->light;

		KU_CORE_ASSERT(l->getId() < MAX_LIGHTS, "Exceeded maximum number of lights");

		bind();

		setUniform("lights[" + std::to_string(l->getId()) + "].type", (int)l->getType());

		setUniform("lights[" + std::to_string(l->getId()) + "].pos", l->getTransform().getPos());
		setUniform("lights[" + std::to_string(l->getId()) + "].dir", l->getTransform().getForward());
		setUniform("lights[" + std::to_string(l->getId()) + "].col", l->getCol());

		setUniform("lights[" + std::to_string(l->getId()) + "].intensity", l->getIntensity());

		setUniform("lights[" + std::to_string(l->getId()) + "].linear", l->getLinear());
		setUniform("lights[" + std::to_string(l->getId()) + "].quadratic", l->getQuadratic());

		setUniform("lights[" + std::to_string(l->getId()) + "].cutoff", glm::cos(glm::radians(l->getAngle())));
	}

	void DefaultShader::updateCamera()
	{
		IShader::updateCamera();

		setUniform("viewPos", shaderData->viewPos);
	}

	const char* SKYBOX_VERT = R"(
		#version 460
		layout (location = 0) in vec3 pos;

		//layout (std140, binding = 0) uniform Matrices
		//{
		//	mat4 projectionMatrix;
		//	mat4 viewMatrix;
		//};

		uniform mat4 projectionMatrix;
		uniform mat4 viewMatrix;		

		out vec3 texCoords;

		void main()
		{
			texCoords = pos;
			vec4 adjustedPos = projectionMatrix * mat4(mat3(viewMatrix)) * vec4(pos, 1.0);
			gl_Position = adjustedPos.xyww;
		}  
	)";

	const char* SKYBOX_FRAG = R"(
		#version 460

		in vec3 texCoords;

		out vec4 fragCol;

		uniform samplerCube skybox;

		void main()
		{    
			fragCol = texture(skybox, texCoords);
		}
	)";

	SkyboxShader::SkyboxShader() : IShader(SKYBOX_VERT, SKYBOX_FRAG)
	{
		bind();

		createUniform("projectionMatrix");
		createUniform("viewMatrix");

		createUniform("skybox");
		setUniform("skybox", 0);
	}

	void StaticShader::init()
	{
		basic = new DefaultShader();
		//ubos["Matrices"] = basic->createUniformBlock("Matrices");

		skybox = new SkyboxShader();
	}

	void StaticShader::cleanup()
	{
		//basic->deleteBuffer(ubos.at("Matrices"));
		delete basic;

		delete skybox;
	}

	uint32_t StaticShader::getUboId(const std::string& name)
	{
		return ubos.at(name);
	}
}


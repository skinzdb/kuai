#include "kpch.h"
#include "StaticShader.h"

using namespace kuai;

Shader* StaticShader::basic = nullptr;
Shader* StaticShader::skybox = nullptr;

const char* DEFAULT_VERT = R"(
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

const char* SHADER_VERT = R"(
	out vec3 texCoords;

	void main()
	{
		texCoords = pos;
		vec4 adjustedPos = projectionMatrix * mat4(mat3(viewMatrix)) * vec4(pos, 1.0);
		gl_Position = adjustedPos.xyww;
	}  
)";

const char* SHADER_FRAG = R"(
	in vec3 texCoords;

	out vec4 fragCol;

	uniform samplerCube skybox;

	void main()
	{    
		fragCol = texture(skybox, texCoords);
	}
)";

DefaultShader::DefaultShader() : Shader(DEFAULT_VERT, DEFAULT_FRAG)
{
	bind();

	createUniform("material.diffuse");
	createUniform("material.specular");
	createUniform("material.shininess");
	setUniform("material.diffuse", 0);
	setUniform("material.specular", 1);

	setUniform("material.shininess", 20.0f);

	createUniform("viewPos");
}

void DefaultShader::update()
{
	//setUniform("material.shininess", shininess);
}

void StaticShader::init()
{
	basic = new DefaultShader();
	skybox = new SkyboxShader();
}

void StaticShader::cleanup()
{
	delete basic;
	delete skybox;
}

SkyboxShader::SkyboxShader() : Shader(SHADER_VERT, SHADER_FRAG)
{
	bind();

	createUniform("skybox");
	setUniform("skybox", 0);
	createUniform("viewPos");
}

void SkyboxShader::update()
{
}

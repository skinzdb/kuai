#include "smpch.h"
#include "StaticShader.h"

const char* PHONG_VERT_SRC = R"(
	#version 330 core
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec3 aNormal;
	layout (location = 2) in vec2 aTexCoords;

	out vec4 worldPos;
	out vec3 worldNormal;
	out vec2 texCoords;

	uniform mat4 projectionMatrix;
	uniform mat4 viewMatrix;
	uniform mat4 modelMatrix;

	uniform mat3 model3x3InvTransp; // Used to calculate proper world position of normals

	void main()
	{
	    worldPos = modelMatrix * vec4(aPos, 1.0);
	    worldNormal = normalize(model3x3InvTransp * aNormal);
	    texCoords = aTexCoords;
	    gl_Position = projectionMatrix * viewMatrix * worldPos;
	}
)";

const char* PHONG_FRAG_SRC = R"(
	#version 330 core
	out vec4 fragCol;

	in vec4 worldPos;
	in vec3 worldNormal;
	in vec2 texCoords;

	uniform vec3 viewPos;
	uniform sampler2D texSampler;

	struct Light
	{
	    vec3 pos;
	    vec3 col;
	    float intensity;
	    float range;
	};
	uniform Light lights[256];

	struct Material
	{
	    vec3 col;
	    sampler2D diffuse;
	    sampler2D specular;
	    float shininess;
	    float reflectivity;
	};
	uniform Material material;

	void main()
	{
	    // Ambient
	    vec3 ambient = material.col;

	    // Diffuse
	    vec3 norm = normalize(worldNormal);
	    vec3 lightDir = normalize(lights[0].pos - worldPos.xyz);
	    float diff = max(dot(norm, lightDir), 0.0);
	    vec3 diffuse = diff * vec3(texture(material.diffuse, texCoords));

	    // Specular
	    vec3 viewDir = normalize(viewPos - worldPos.xyz);
	    vec3 reflectDir = reflect(-lightDir, norm);
	    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	    vec3 specular = vec3(material.reflectivity) * spec * vec3(texture(material.specular, texCoords));

	    vec3 finalCol = lights[0].intensity * lights[0].col * (diffuse + specular);

	    fragCol = vec4(finalCol, 1.0);
	}
)";
	

namespace Smong {
	PhongShader* StaticShader::Phong = nullptr;

	void StaticShader::Init()
	{
		Phong = new PhongShader();
	}

	void StaticShader::Cleanup()
	{
		delete Phong;
	}

	// *** Phong Shader Methods ***********************************************

	PhongShader::PhongShader() : Shader(PHONG_VERT_SRC, PHONG_FRAG_SRC)
	{
		Bind();

		CreateUniform("projectionMatrix");
		CreateUniform("viewMatrix");
		CreateUniform("modelMatrix");

		CreateUniform("model3x3InvTransp");

		CreateUniform("texSampler");
		SetUniform("texSampler", 0);

		CreateUniform("viewPos");

		CreateUniform("lights");
		CreateUniform("material");
	}

	void PhongShader::SetUniforms(glm::vec3& col, float shininess, float reflectivity)
	{
		SetUniform("material.col", col);
		SetUniform("material.shininess", shininess);
		SetUniform("material.reflectivity", reflectivity);
	}
}



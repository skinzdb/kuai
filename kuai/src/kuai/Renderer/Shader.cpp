#include "kpch.h"
#include "Shader.h"

#include "kuai/Components/Entity.h"
#include "kuai/Components/Components.h"

namespace kuai {
	std::string VERT_HEADER = R"(
		#version 460
		layout (location = 0) in vec3 pos;
		layout (location = 1) in vec3 normal;
		layout (location = 2) in vec2 texCoord;

		//layout (shared) uniform Matrices
		//{
		//	mat4 projectionMatrix;
		//	mat4 viewMatrix;
		//};

		uniform mat4 projectionMatrix;
		uniform mat4 viewMatrix;

		uniform mat4 modelMatrix;
		uniform mat3 model3x3InvTransp; // Used to calculate proper world position of normals		
	)";

	std::string FRAG_HEADER = R"(
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
	)";

	Shader::Shader(const std::string& vertSrc, const std::string& fragSrc)
		: ShaderProgram(VERT_HEADER + vertSrc, FRAG_HEADER + fragSrc)
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
	}

	Shader::~Shader()
	{
	}

	void Shader::setLight(Entity* light)
	{
		Light l = light->getComponent<Light>();
		uint32_t id = l.getId();

		KU_CORE_ASSERT(id < MAX_LIGHTS, "Exceeded maximum number of lights");

		bind();

		setUniform("lights[" + std::to_string(id) + "].type", (int)l.getType());
											   
		setUniform("lights[" + std::to_string(id) + "].pos", light->getTransform().getPos());
		setUniform("lights[" + std::to_string(id) + "].dir", light->getTransform().getForward());
		setUniform("lights[" + std::to_string(id) + "].col", l.getCol());
											   
		setUniform("lights[" + std::to_string(id) + "].intensity", l.getIntensity());
											   
		setUniform("lights[" + std::to_string(id) + "].linear", l.getLinear());
		setUniform("lights[" + std::to_string(id) + "].quadratic", l.getQuadratic());
											   
		setUniform("lights[" + std::to_string(id) + "].cutoff", glm::cos(glm::radians(l.getAngle())));
	}
}


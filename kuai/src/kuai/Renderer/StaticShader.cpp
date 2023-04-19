#include "kpch.h"
#include "StaticShader.h"

#include "kuai/Components/Components.h"

namespace kuai {
	Shader* StaticShader::basic = nullptr;
	Shader* StaticShader::skybox = nullptr;
	Shader* StaticShader::depth = nullptr;
	
	const char* DEFAULT_VERT = R"(
		#version 450
		layout (location = 0) in vec3 pos;
		layout (location = 1) in vec3 normal;
		layout (location = 2) in vec2 texCoord;

		layout (binding = 0) uniform Matrices
		{
			mat4 projectionMatrix;
			mat4 viewMatrix;
		};		

		uniform mat4 modelMatrix;
		uniform mat3 model3x3InvTransp; // Used to calculate proper world position of normals	

		uniform mat4 lightSpaceMatrix;	

		out vec4 worldPos;
		out vec3 worldNorm;
		out vec2 texCoords;
		out vec4 lightSpace;

		void main()
		{
			worldPos = modelMatrix * vec4(pos, 1.0);
			worldNorm = model3x3InvTransp * normal;
			texCoords = texCoord;
			lightSpace = lightSpaceMatrix * worldPos;

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

			int castShadows;
		};

		uniform Light lights[MAX_LIGHTS];
		// layout (binding = 1) uniform Lights
		// {
		// 	Light lights[MAX_LIGHTS];
		// };

		out vec4 fragCol;

		in vec4 worldPos;
		in vec3 worldNorm;
		in vec2 texCoords;
		in vec4 lightSpace;

		uniform vec3 viewPos;

		struct Material
		{
			sampler2D diffuse;
			sampler2D specular;
			float shininess;
		};
		uniform Material material;

		uniform sampler2D shadowMap;

		float calcShadow(vec4 lightSpace, vec3 lightDir)
		{
			// Perform perspective divide and transform to NDC coords
  			vec3 projCoords = lightSpace.xyz / lightSpace.w;
			projCoords = projCoords * 0.5 + 0.5;		

			float closestDepth = texture(shadowMap, projCoords.xy).r;
			float currentDepth = projCoords.z;
			
			// Add bias to remove shadow acne
			float bias = max(0.05 * (1.0 - dot(worldNorm, lightDir)), 0.005);  

			// Use PCF to sample current texel and 8 surrounding texels to create smoother shadows
			float shadow = 0.0;
			vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
			for(int x = -1; x <= 1; x++)
			{
				for(int y = -1; y <= 1; y++)
				{
					float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
					shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
				}    
			}
			shadow /= 9.0;

			if(projCoords.z > 1.0) // No shadows if fragment is past far plane
        		shadow = 0.0;

			return shadow;
		}

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
					//lightDir = normalize(lights[i].pos - worldPos.xyz);
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

				float shadow = 0.0;
				if (lights[i].castShadows)
				{
					shadow = calcShadow(lightSpace, lightDir);      
				}
				 
				finalCol += (ambient + (1.0 - shadow) * (diffuse + specular)) * factor;
			}

			fragCol = vec4(finalCol, 1.0);
		}
	)";

	DefaultShader::DefaultShader() : Shader(DEFAULT_VERT, DEFAULT_FRAG)
	{
		bind();

		createUniformBlock("Matrices", { "projectionMatrix", "viewMatrix" }, 0);
		
		// std::vector<std::string> lightNames;
		// std::vector<const char*> lightNamesC;
		// for (size_t i = 0; i < MAX_LIGHTS; i++)	// TODO: HORRIBLE DISGUSTING CODE
		// {
		// 	lightNames.push_back("lights[" + std::to_string(i) + "].type");
		// 	lightNames.push_back("lights[" + std::to_string(i) + "].pos");
		// 	lightNames.push_back("lights[" + std::to_string(i) + "].dir");
		// 	lightNames.push_back("lights[" + std::to_string(i) + "].col");
		// 	lightNames.push_back("lights[" + std::to_string(i) + "].intensity");
		// 	lightNames.push_back("lights[" + std::to_string(i) + "].linear");
		// 	lightNames.push_back("lights[" + std::to_string(i) + "].quadratic");
		// 	lightNames.push_back("lights[" + std::to_string(i) + "].cutoff");
		// }
		// for (auto& name : lightNames)
		// 	lightNamesC.push_back(name.c_str());
		// createUniformBlock("Lights", lightNamesC, 1);

		for (size_t i = 0; i < MAX_LIGHTS; i++)
		{
			createUniform("lights[" + std::to_string(i) + "].type");
			createUniform("lights[" + std::to_string(i) + "].pos");
			createUniform("lights[" + std::to_string(i) + "].dir");
			createUniform("lights[" + std::to_string(i) + "].col");
			createUniform("lights[" + std::to_string(i) + "].intensity");
			createUniform("lights[" + std::to_string(i) + "].linear");
			createUniform("lights[" + std::to_string(i) + "].quadratic");
			createUniform("lights[" + std::to_string(i) + "].cutoff");
			createUniform("lights[" + std::to_string(i) + "].castShadows");
		}

		createUniform("modelMatrix");
		createUniform("model3x3InvTransp");

		createUniform("material.diffuse");
		createUniform("material.specular");
		setUniform("material.diffuse", 0);
		setUniform("material.specular", 1);

		createUniform("material.shininess");
		setUniform("material.shininess", 35.0f);

		createUniform("lightSpaceMatrix");
		createUniform("shadowMap");
		setUniform("shadowMap", 7);

		createUniform("viewPos");
	}

	const char* SKYBOX_VERT = R"(
		#version 450
		layout (location = 0) in vec3 pos;

		layout (std140, binding = 0) uniform Matrices
		{
			mat4 projectionMatrix;
			mat4 viewMatrix;
		};	

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

	SkyboxShader::SkyboxShader() : Shader(SKYBOX_VERT, SKYBOX_FRAG)
	{
		bind();

		createUniform("skybox");
		setUniform("skybox", 0);
	}

	const char* DEPTH_VERT = R"(
		#version 450
		layout (location = 0) in vec3 pos;

		uniform mat4 lightSpaceMatrix;
		uniform mat4 modelMatrix;

		void main()
		{
			gl_Position = lightSpaceMatrix * modelMatrix * vec4(pos, 1.0);
		}
	)";

	const char* DEPTH_FRAG = R"(
		#version 450
		// out float fragDepth;
		void main()
		{
			// fragDepth = gl_FragCoord.z; (Handled by OpenGL)
		}
	)";

	DepthShader::DepthShader() : Shader(DEPTH_VERT, DEPTH_FRAG)
	{
		bind();

		createUniform("lightSpaceMatrix");
		createUniform("modelMatrix");
	}

	void StaticShader::init()
	{
		basic = new DefaultShader();
		skybox = new SkyboxShader();
		depth = new DepthShader();
	}

	void StaticShader::cleanup()
	{
		//delete basic;
		//delete skybox;
	}
}


#include "kpch.h"
#include "StaticShader.h"

#include "kuai/Components/Components.h"

namespace kuai {
	Shader* StaticShader::basic = nullptr;
	Shader* StaticShader::skybox = nullptr;
	Shader* StaticShader::depth = nullptr;
	
	const char* DEFAULT_VERT = R"(
		#version 450
		#define MAX_LIGHTS 10

		layout (location = 0)	in vec3 aPos;
		layout (location = 1)	in vec3 aNormal;
		layout (location = 2)	in vec2 aTexCoord;
		layout (location = 3)   in int  aMatId;
		layout (location = 4)	in mat4 aModelMatrix;
		// layout (location = 8) in mat3 aModel3x3InvTransp;

		layout (binding = 0) uniform CamData
		{
			mat4 projectionMatrix;
			mat4 viewMatrix;
			vec3 viewPos;
		};	

		uniform mat4 lightSpaceMatrix; // TODO: change to layout, have an array of matrices to handle shadows from multiple lights

		out vec4 worldPos;
		out vec3 worldNorm;
		out vec2 texCoords;
		out vec4 lightSpace;		   // TODO: change to array
		out vec3 viewingPos;
		out flat int matId;

		void main()
		{
			worldPos = aModelMatrix * vec4(aPos, 1.0);
			mat3 model3x3InvTransp = mat3(transpose(inverse(aModelMatrix))); // TODO: remove in favour of vertex attribute
			worldNorm = model3x3InvTransp * aNormal;
			texCoords = aTexCoord;
			matId = aMatId;
			lightSpace = lightSpaceMatrix * worldPos;
			viewingPos = viewPos;

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

			int castsShadows;
		};

		layout (binding = 1) uniform Lights
		{
			Light lights[MAX_LIGHTS];
		};

		out vec4 fragCol;

		in vec4 worldPos;
		in vec3 worldNorm;
		in vec2 texCoords;
		in vec4 lightSpace;		// TODO: change to array
		in vec3 viewingPos;
		in flat int matId;

		struct Material
		{
			sampler2D diffuse;
			sampler2D specular;
			samplerCube reflectionMap;
			int reflections;
			float shininess;
		};
		uniform	Material materials[10];

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
					shadow += currentDepth - bias > pcfDepth ? 0.75 : 0.0;        
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
			vec3 viewDir = normalize(viewingPos - worldPos.xyz);
			vec3 viewReflectDir = reflect(-viewDir, norm);
			vec3 lightDir = vec3(0.0, 0.0, 0.0);
			float factor = 1.0;

			vec3 finalCol = vec3(0.0, 0.0, 0.0);

			for (int i = 0; i < 1; i++)
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
					factor *= lights[i].intensity;

					if (lights[i].type == 2) // Spotlight
					{
						float theta = dot(lightDir, normalize(-lights[i].dir)); 
						float epsilon = 0.1;
						float intensity = clamp((theta - lights[i].cutoff) / epsilon, 0.0, 1.0);

						factor *= intensity;
					}
				}

				// Ambient
				vec3 ambient = 0.1 * lights[i].col;// vec3(texture(materials[matId].diffuse, texCoords));

				// Diffuse
				float diff = max(dot(norm, lightDir), 0.0);
				vec3 diffuse = lights[i].col * diff;//vec3(texture(materials[matId].diffuse, texCoords));

				// Specular
				vec3 reflectDir = reflect(-lightDir, norm);
				vec3 halfwayDir = normalize(lightDir + viewDir);
				float spec = pow(max(dot(norm, halfwayDir), 0.0), materials[matId].shininess);
				vec3 specular = spec * vec3(1.0,1.0,1.0);//vec3(texture(materials[matId].specular, texCoords));

				// Reflection Map
				vec3 reflection = vec3(0.0);
				if (materials[matId].reflections)
				{
					reflection = vec3(texture(materials[matId].reflectionMap, viewReflectDir));
				}
				
				float shadow = 0.0;
				if (lights[i].castsShadows)
				{
					shadow = calcShadow(lightSpace, lightDir);      
				}
				 
				finalCol += (ambient + (1.0 - shadow) * (diffuse + specular + reflection)) * factor;
			}

			fragCol = vec4(finalCol, 1.0);
		}
	)";

	DefaultShader::DefaultShader() : Shader(DEFAULT_VERT, DEFAULT_FRAG) 
	{
		Rc<VertexBuffer> vbo1 = MakeRc<VertexBuffer>(0);
		Rc<VertexBuffer> vbo2 = MakeRc<VertexBuffer>(0);

		vbo1->setLayout(
		{ 
			{ ShaderDataType::VEC3, "pos" },
			{ ShaderDataType::VEC3, "normal" },
			{ ShaderDataType::VEC2, "texCoord" },
			{ ShaderDataType::INT,  "texId" }
		});
		vbo2->setLayout(
		{
			{ ShaderDataType::MAT4,  "modelMatrix" }, // TODO: change to 3 Vec4s and use fact that bottom row of model matrix is always (0, 0, 0, 1)
			// { ShaderDataType::MAT3,  "model3x3InvTransp" }
		});
		vao->addVertexBuffer(vbo1);
		vao->addVertexBuffer(vbo2);

		bind();

		createUniformBlock("CamData", { "projectionMatrix", "viewMatrix", "viewPos"}, 0);
		
		std::vector<std::string> lightNames;
		std::vector<const char*> lightNamesCStr;
		for (u8 i = 0; i < MAX_LIGHTS; i++)	// TODO: HORRIBLE DISGUSTING CODE
		{
			lightNames.push_back("lights[" + std::to_string(i) + "].type");
		 	lightNames.push_back("lights[" + std::to_string(i) + "].pos");
		 	lightNames.push_back("lights[" + std::to_string(i) + "].dir");
		 	lightNames.push_back("lights[" + std::to_string(i) + "].col");
		 	lightNames.push_back("lights[" + std::to_string(i) + "].intensity");
		 	lightNames.push_back("lights[" + std::to_string(i) + "].linear");
		 	lightNames.push_back("lights[" + std::to_string(i) + "].quadratic");
		 	lightNames.push_back("lights[" + std::to_string(i) + "].cutoff");
			lightNames.push_back("lights[" + std::to_string(i) + "].castsShadows");
		}
		for (auto& name : lightNames)
			lightNamesCStr.push_back(name.c_str());
		createUniformBlock("Lights", lightNamesCStr, 1);

		for (u8 i = 0; i < 10; i++)	
		{
			createUniform("materials[" + std::to_string(i) + "].diffuse");
			createUniform("materials[" + std::to_string(i) + "].specular");
			createUniform("materials[" + std::to_string(i) + "].reflectionMap");

			setUniform("materials[" + std::to_string(i) + "].diffuse", i * 3);
			setUniform("materials[" + std::to_string(i) + "].specular", i * 3 + 1);
			setUniform("materials[" + std::to_string(i) + "].reflectionMap", i * 3 + 2);

			createUniform("materials[" + std::to_string(i) + "].shininess");
			createUniform("materials[" + std::to_string(i) + "].reflections");
			setUniform("materials[" + std::to_string(i) + "].shininess", 20.0f);
			setUniform("materials[" + std::to_string(i) + "].reflections", 0);
		}

		createUniform("shadowMap");
		setUniform("shadowMap", 30);
	}

	const char* SKYBOX_VERT = R"(
		#version 450
		layout (location = 0) in vec3 pos;

		layout (binding = 0) uniform CamData
		{
			mat4 projectionMatrix;
			mat4 viewMatrix;
			vec3 viewPos;
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

		layout (binding = 2) uniform sampler2D textures[32];

		uniform samplerCube skybox;

		void main()
		{    
			fragCol = texture(skybox, texCoords);
		}
	)";

	SkyboxShader::SkyboxShader() : Shader(SKYBOX_VERT, SKYBOX_FRAG)
	{
		Rc<VertexBuffer> vbo1 = MakeRc<VertexBuffer>(0);
		Rc<VertexBuffer> vbo2 = MakeRc<VertexBuffer>(0);

		vbo1->setLayout({ { ShaderDataType::VEC3, "pos" } });
		vbo2->setLayout({ { ShaderDataType::MAT4, "modelMatrix" } });

		vao->addVertexBuffer(vbo1);
		vao->addVertexBuffer(vbo2);

		bind();

		createUniform("skybox");
		setUniform("skybox", 0);
	}

	const char* DEPTH_VERT = R"(
		#version 450
		layout (location = 0) in vec3 pos;
		layout (location = 1) in mat4 modelMatrix;
		layout (location = 5) in mat4 lightSpaceMatrix;

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
		Rc<VertexBuffer> vbo1 = MakeRc<VertexBuffer>(0);
		Rc<VertexBuffer> vbo2 = MakeRc<VertexBuffer>(0);

		vbo1->setLayout({ { ShaderDataType::VEC3, "pos" } });
		vbo2->setLayout(
		{ 
			{ ShaderDataType::MAT4, "modelMatrix" },
			{ ShaderDataType::MAT4, "lightSpaceMatrix" }
		});

		vao->addVertexBuffer(vbo1);
		vao->addVertexBuffer(vbo2);

		bind();
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


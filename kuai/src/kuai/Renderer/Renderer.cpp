#include "kpch.h"

#include "Renderer.h"
#include "StaticShader.h"

#include "glad/glad.h"

#include "glm/gtc/matrix_inverse.hpp"

namespace kuai {
    std::unique_ptr<Renderer::RenderData> Renderer::renderData = std::make_unique<Renderer::RenderData>();
    std::shared_ptr<Framebuffer> Renderer::framebuffer = nullptr;
    std::unique_ptr<Framebuffer> Renderer::shadowMap = nullptr;

    void Renderer::init()
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_FRAMEBUFFER_SRGB); // TODO: IMPLEMENT THIS MANUALLY IN SHADER AND TEXTURES

        shadowMap = std::make_unique<Framebuffer>(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 1, 0);
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, shadowMap->getDepthAttachment());

        StaticShader::init();
    }

    void Renderer::cleanup()
    {
        StaticShader::cleanup();
    }

    void Renderer::setCamera(CameraComponent& camera)
    {
        // framebuffer = camera.cam.getTarget();
    }

    void Renderer::updateShadowMap(Light light)
    {
        if (!light.castsShadows())
            return;
        
        StaticShader::depth->bind();
        StaticShader::depth->setUniform("lightSpaceMatrix", light.getLightSpaceMatrix());
		
		shadowMap->bind();
        glViewport(
            (light.getId() % LIGHTS_PER_ROW) * LIGHT_SHADOW_SIZE, 
            (light.getId() / LIGHTS_PER_ROW) * LIGHT_SHADOW_SIZE, 
            LIGHT_SHADOW_SIZE, 
            LIGHT_SHADOW_SIZE
        );
        glClear(GL_DEPTH_BUFFER_BIT);
        // glCullFace(GL_FRONT); // Helps avoid peter-panning; however completely removes objects with no back faces :(
        renderDepth();
        // glCullFace(GL_BACK);
        shadowMap->unbind();
    }

    void Renderer::setLights(const std::vector<std::shared_ptr<Entity>>& lightEntities)
    {
        renderData->lightEntities = lightEntities;
    }

    void Renderer::setMeshes(const std::vector<std::shared_ptr<Entity>>& renderEntities)
    {
        renderData->renderEntities = renderEntities;
    }

    void Renderer::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);
    }

    void Renderer::setClearCol(const glm::vec4& col)
    {
        glClearColor(col.r, col.g, col.b, col.a);
    }

    void Renderer::clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::render()
    {
        KU_PROFILE_FUNCTION();

        for (auto& renderEntity : renderData->renderEntities)
        {
            for (auto& mesh : renderEntity->getComponent<MeshRenderer>().getModel()->getMeshes())
            {
                Shader* shader = mesh->getMaterial()->getShader();
                shader->bind();

				if (shader == StaticShader::basic)
				{
                    DefaultMaterial* mat = (DefaultMaterial*)mesh->getMaterial().get();
					shader->setUniform("modelMatrix", renderEntity->getTransform().getModelMatrix());
					shader->setUniform("model3x3InvTransp", glm::inverseTranspose(glm::mat3(renderEntity->getTransform().getModelMatrix())));
                    shader->setUniform("material.reflections", mat->reflections);
                    shader->setUniform("material.shininess", mat->specularAmount);
				}
                mesh->render();
            }
        }
    }

    void Renderer::renderDepth()
    {
        StaticShader::depth->bind();
        for (auto& renderEntity : renderData->renderEntities)
        {
            MeshRenderer renderer = renderEntity->getComponent<MeshRenderer>();
            if (!renderer.castsShadows())
                continue;
            
            for (auto& mesh : renderEntity->getComponent<MeshRenderer>().getModel()->getMeshes())
            {
                    if (mesh->getMaterial()->getShader() == StaticShader::skybox)
                        continue;
                    StaticShader::depth->setUniform("modelMatrix", renderEntity->getTransform().getModelMatrix());
                    mesh->render();
            }
            
        } 
    }
}



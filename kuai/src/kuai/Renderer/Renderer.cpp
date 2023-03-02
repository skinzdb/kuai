#include "kpch.h"
#include "Renderer.h"

#include "StaticShader.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_inverse.hpp>

namespace kuai {
    std::unique_ptr<Renderer::CameraData> Renderer::camData = std::make_unique<Renderer::CameraData>();
    std::vector<std::shared_ptr<Entity>> Renderer::lights = std::vector<std::shared_ptr<Entity>>();

    void Renderer::init()
    {
        StaticShader::init();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_FRAMEBUFFER_SRGB); // TODO: IMPLEMENT THIS MANUALLY IN SHADER AND TEXTURES 
    }

    void Renderer::cleanup()
    {
        StaticShader::cleanup();
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

    void Renderer::setCamera(Camera& cam, glm::vec3& viewPos)
    {
        camData->projectionMatrix = cam.getProjectionMatrix();
        camData->viewMatrix = cam.getViewMatrix();
        camData->viewPos = viewPos;
    }

    void Renderer::setLights(std::vector<std::shared_ptr<Entity>>& lights)
    {
        Renderer::lights = lights;
    }

    void Renderer::render(MeshMaterial& meshMat, glm::mat4& transform)
    {
        Shader* shader = meshMat.material->shader;

        shader->bind();

        for (int i = 0; i < lights.size(); i++)
        {
            Light light = lights[i]->getComponent<Light>();

            shader->setUniform("lights[" + std::to_string(i) + "].type", (int)light.getType());

            shader->setUniform("lights[" + std::to_string(i) + "].pos", lights[i]->getTransform().getPos());
            shader->setUniform("lights[" + std::to_string(i) + "].dir", lights[i]->getTransform().getRot());
            shader->setUniform("lights[" + std::to_string(i) + "].col", light.getCol());

            shader->setUniform("lights[" + std::to_string(i) + "].intensity", light.getIntensity());

            shader->setUniform("lights[" + std::to_string(i) + "].linear", light.getLinear());
            shader->setUniform("lights[" + std::to_string(i) + "].quadratic", light.getQuadratic());

            shader->setUniform("lights[" + std::to_string(i) + "].cutoff", glm::cos(glm::radians(light.getAngle())));
        }

        shader->setUniform("projectionMatrix", camData->projectionMatrix);
        shader->setUniform("viewMatrix", camData->viewMatrix);
        shader->setUniform("viewPos", camData->viewPos);
              
        shader->setUniform("modelMatrix", transform);
        shader->setUniform("model3x3InvTransp", glm::inverseTranspose(glm::mat3(transform)));
           
        meshMat.Render();
    }
}



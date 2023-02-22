#include "smpch.h"
#include "Renderer.h"

#include "StaticShader.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_inverse.hpp>

namespace Smong {
    std::unique_ptr<Renderer::CameraData> Renderer::camData = std::make_unique<Renderer::CameraData>();
    std::vector<std::shared_ptr<Entity>> Renderer::lights = std::vector<std::shared_ptr<Entity>>();

    void Renderer::Init()
    {
        StaticShader::Init();

        // Don't draw faces that are pointing away from the camera
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_FRAMEBUFFER_SRGB); // TODO: IMPLEMENT THIS MANUALLY IN SHADER AND TEXTURES 
    }

    void Renderer::Cleanup()
    {
        StaticShader::Cleanup();
    }

    void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);
    }

    void Renderer::SetClearCol(const glm::vec4& col)
    {
        glClearColor(col.r, col.g, col.b, col.a);
    }

    void Renderer::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::SetCamera(Camera& cam, glm::vec3& viewPos)
    {
        camData->projectionMatrix = cam.GetProjectionMatrix();
        camData->viewMatrix = cam.GetViewMatrix();
        camData->viewPos = viewPos;
    }

    void Renderer::SetLights(std::vector<std::shared_ptr<Entity>>& lights)
    {
        Renderer::lights = lights;
    }

    void Renderer::Render(MeshMaterial& meshMat, glm::mat4& transform)
    {
        Shader* shader = meshMat.material->shader;

        shader->Bind();

        for (int i = 0; i < lights.size(); i++)
        {
            Light light = lights[i]->GetComponent<Light>();

            shader->SetUniform("lights[" + std::to_string(i) + "].type", (int)light.type);

            shader->SetUniform("lights[" + std::to_string(i) + "].pos", lights[i]->GetTransform().pos);
            shader->SetUniform("lights[" + std::to_string(i) + "].dir", lights[i]->GetTransform().rot);
            shader->SetUniform("lights[" + std::to_string(i) + "].col", light.col);

            shader->SetUniform("lights[" + std::to_string(i) + "].intensity", light.intensity);

            shader->SetUniform("lights[" + std::to_string(i) + "].linear", light.linear);
            shader->SetUniform("lights[" + std::to_string(i) + "].quadratic", light.quadratic);

            shader->SetUniform("lights[" + std::to_string(i) + "].cutoff", glm::cos(glm::radians(light.angle)));
        }

        shader->SetUniform("projectionMatrix", camData->projectionMatrix);
        shader->SetUniform("viewMatrix", camData->viewMatrix);
        shader->SetUniform("viewPos", camData->viewPos);
              
        shader->SetUniform("modelMatrix", transform);
        shader->SetUniform("model3x3InvTransp", glm::inverseTranspose(glm::mat3(transform)));
           
        meshMat.Render();
    }
}



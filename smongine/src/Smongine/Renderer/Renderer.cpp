#include "smpch.h"
#include "Renderer.h"

#include "StaticShader.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Smong {
    std::shared_ptr<Mesh> mesh;
    Texture* texture;

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
    }

    void Renderer::Cleanup()
    {
        StaticShader::Cleanup();
    }

    void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);
    }

    void Renderer::SetClearCol(glm::vec4& col)
    {
        glClearColor(col.r, col.g, col.b, col.a);
    }

    void Renderer::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::Render(Scene& scene)
    {
        for (auto& entityId : scene.GetRenderItems())
        {
            Entity* renderEntity = scene.GetEntityById(entityId);

            MeshMaterial meshMat = renderEntity->GetComponent<MeshMaterial>();
            Shader* shader = meshMat.material.shader;

            shader->Bind();

            shader->SetUniform("projectionMatrix", scene.GetMainCam().GetProjectionMatrix());
            shader->SetUniform("viewMatrix", scene.GetMainCam().GetViewMatrix());
            shader->SetUniform("modelMatrix", renderEntity->GetTransform().GetModelMatrix());

            shader->SetUniform("model3x3InvTransp", glm::inverseTranspose(glm::mat3(renderEntity->GetTransform().GetModelMatrix())));

            shader->SetUniform("viewPos", glm::vec3(0, 0, 0));

            for (int i = 0; i < scene.GetLights().size(); i++)
            {
                Entity* lightEntity = scene.GetEntityById(scene.GetLights()[i]);
                Light light = lightEntity->GetComponent<Light>();

                shader->SetUniform("lights[" + std::to_string(i) + "].type", light.type);
            
                shader->SetUniform("lights[" + std::to_string(i) + "].pos", lightEntity->GetTransform().GetPos());
                shader->SetUniform("lights[" + std::to_string(i) + "].dir", lightEntity->GetTransform().GetForward());
                shader->SetUniform("lights[" + std::to_string(i) + "].col", light.col);

                shader->SetUniform("lights[" + std::to_string(i) + "].intensity", light.intensity);

                shader->SetUniform("lights[" + std::to_string(i) + "].linear", light.linear);
                shader->SetUniform("lights[" + std::to_string(i) + "].quadratic", light.quadratic);

                //shader->SetUniform("lights[" + std::to_string(i) + "].cutoff", light.angle);
            }

            meshMat.Render();
        }
    }
}



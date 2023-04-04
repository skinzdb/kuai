#include "kpch.h"
#include "Renderer.h"

#include <glad/glad.h>

#include "StaticShader.h"

namespace kuai {
    std::unique_ptr<Renderer::CameraData> Renderer::camData = std::make_unique<Renderer::CameraData>();

    void Renderer::init()
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_CULL_FACE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_FRAMEBUFFER_SRGB); // TODO: IMPLEMENT THIS MANUALLY IN SHADER AND TEXTURES 

        StaticShader::init();
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

    void Renderer::render(Mesh& mesh)
    {
        KU_PROFILE_FUNCTION();

        mesh.getMaterial()->getShader()->bind();
        mesh.render();
    }
}



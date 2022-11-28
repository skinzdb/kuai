#include "smpch.h"
#include "Renderer.h"

#include "Smongine/Util/FileUtil.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"

#include <glad/glad.h>
#include <stb_image.h>

float vertices[] =
{
    -1, -1, -1,
    1, -1, -1,
    1, 1, -1,
    -1, 1, -1,
    -1, -1, 1,
    1, -1, 1,
    1, 1, 1,
    -1, 1, 1
};

float texCoords[] =
{
    0, 0,
    1, 0,
    1, 1,
    0, 1
};

uint32_t indices[] =
{
    0, 1, 3, 3, 1, 2,
    1, 5, 2, 2, 5, 6,
    5, 4, 6, 6, 4, 7,
    4, 0, 7, 7, 0, 3,
    3, 2, 7, 7, 2, 6,
    4, 5, 0, 0, 5, 1
};

namespace Smong {
    Mesh* mesh;
    Texture* texture;
    Shader* shader;

    void Renderer::Init()
    {
        //// Don't draw faces that are pointing away from the camera
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_BACK);

        glEnable(GL_DEPTH_TEST);

        std::string vertShaderSrc = FileUtil::Load("C:/Users/David/source/repos/smongine/Smongine/src/Smongine/shader.vert");
        std::string fragShaderSrc = FileUtil::Load("C:/Users/David/source/repos/smongine/Smongine/src/Smongine/shader.frag");

        shader = new Shader(vertShaderSrc.c_str(), fragShaderSrc.c_str());

        texture = new Texture("C:/Users/David/Pictures/adam_4.png");

        mesh = new Mesh(vertices, 12, texCoords, 8, indices, 6);

        shader->CreateUniform("texSampler");
    }

    void Renderer::Cleanup()
    {
        delete texture;
        delete mesh;
        delete shader;
    }

    void Renderer::Render()
    {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->Bind();
        shader->SetUniform("texSampler", 0);

        texture->Bind();

        mesh->Render();
      
        shader->Unbind();
    }
}



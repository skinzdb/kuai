#include "kpch.h"
#include "Skybox.h"

#include "Geometry.h"
#include "glad/glad.h"

static std::vector<float> positions = 
{       
    -1.0f,  1.0f, -1.0f, 
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

static std::vector<uint32_t> indices = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 };

namespace kuai {
    Rc<Mesh> Skybox::mesh = MakeRc<Mesh>(positions, std::vector<float>(), std::vector<float>(), indices);

	Skybox::Skybox(Rc<Cubemap> cubemap) : Model(Skybox::mesh, MakeRc<SkyboxMaterial>(cubemap))
	{
	}

    Skybox::~Skybox()
    {
    }

    // void Skybox::render()
    // {
    //     // Draw skybox last

    //     // Change depth function so depth test passes when values are equal to depth buffer's content
    //     // glDepthFunc(GL_LEQUAL);

    //     material->render();

    //     //glBindVertexArray(vaoId);
    //     //glEnableVertexAttribArray(0);
    //     //glEnableVertexAttribArray(1);
    //     //glEnableVertexAttribArray(2);

    //     //glDrawArrays(GL_TRIANGLES, 0, 36);

    //     // Restore state
    //     //glDisableVertexAttribArray(0);
    //     //glDisableVertexAttribArray(1);
    //     //glDisableVertexAttribArray(2);

    //     //glBindVertexArray(GL_NONE); // No need to unbind every time
    //     //glBindTexture(GL_TEXTURE_2D, GL_NONE);

    //     // Revert depth function to normal
    //     //glDepthFunc(GL_LESS);
    // }
}

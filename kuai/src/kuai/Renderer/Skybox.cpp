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

namespace kuai {
    Rc<Mesh> Skybox::mesh = MakeRc<Mesh>(positions, std::vector<float>(), std::vector<float>(), std::vector<u32>());

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

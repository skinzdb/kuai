#include "kpch.h"

#include "Renderer.h"
#include "StaticShader.h"

#include "glad/glad.h"

#include "glm/gtc/matrix_inverse.hpp"

namespace kuai {
    Box<Renderer::RenderData> Renderer::renderData = std::make_unique<Renderer::RenderData>();
    Rc<Framebuffer> Renderer::framebuffer = nullptr;
    Box<Framebuffer> Renderer::shadowMap =  nullptr;

    void Renderer::init()
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        i32 textureUnits;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
        KU_CORE_WARN("Max texture units: {0}", textureUnits);

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

    void Renderer::setCamera(Camera& camera)
    {
        StaticShader::basic->setUniform("Matrices", "projectionMatrix", &camera.getProjectionMatrix()[0][0], sizeof(glm::mat4));
        StaticShader::basic->setUniform("Matrices", "viewMatrix", &camera.getViewMatrix()[0][0], sizeof(glm::mat4));
        // framebuffer = camera.cam.getTarget();
    }

    void Renderer::updateShadowMap(Light& light)
    {
        if (!light.castsShadows())
            return;
       
		shadowMap->bind();
        glViewport(
            (light.getId() % LIGHTS_PER_ROW) * LIGHT_SHADOW_SIZE, 
            (light.getId() / LIGHTS_PER_ROW) * LIGHT_SHADOW_SIZE, 
            LIGHT_SHADOW_SIZE, 
            LIGHT_SHADOW_SIZE
        );
        glClear(GL_DEPTH_BUFFER_BIT);
        // glCullFace(GL_FRONT); // Helps avoid peter-panning; however completely removes objects with no back faces :(
        render(*StaticShader::depth);
        // glCullFace(GL_BACK);
        shadowMap->unbind();
    }

    void Renderer::render(Shader& shader)
    {
        shader.bind();

        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, (void*)0, shader.getCommandCount(), sizeof(IndirectCommand));
    }

    void Renderer::setViewport(u32 x, u32 y, u32 width, u32 height)
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

    //void Renderer::render()
    //{
    //    KU_PROFILE_FUNCTION();
    //
    //    for (auto& renderEntity : renderData->renderEntities)
    //    {
    //        if (!renderEntity->getComponent<MeshRenderer>().getModel())
	//			continue;
    //        for (auto& mesh : renderEntity->getComponent<MeshRenderer>().getModel()->getMeshes())
    //        {
    //            Shader* shader = mesh->getMaterial()->getShader();
    //            shader->bind();
    //
	//			if (shader == StaticShader::basic)
	//			{
    //              DefaultMaterial* mat = (DefaultMaterial*)mesh->getMaterial().get();
	//			    shader->setUniform("modelMatrix", renderEntity->getTransform().getModelMatrix());
	//				shader->setUniform("model3x3InvTransp", glm::inverseTranspose(glm::mat3(renderEntity->getTransform().getModelMatrix())));
    //              shader->setUniform("material.reflections", mat->reflections);
    //              shader->setUniform("material.shininess", mat->specularAmount);
	//			}
    //            mesh->render();
    //        }
    //    }
    //}

    //void Renderer::renderDepth()
    //{
    //    StaticShader::depth->bind();

    //    for (auto& renderEntity : renderData->renderEntities)
    //    {
    //        MeshRenderer renderer = renderEntity->getComponent<MeshRenderer>();
    //        if (!renderer.getModel() || !renderer.castsShadows())
    //            continue;
    //        
    //        for (auto& mesh : renderEntity->getComponent<MeshRenderer>().getModel()->getMeshes())
    //        {
    //                if (mesh->getMaterial()->getShader() == StaticShader::skybox)
    //                    continue;
    //                StaticShader::depth->setUniform("modelMatrix", renderEntity->getTransform().getModelMatrix());
    //                mesh->render();
    //        }
    //        
    //    } 
    //}
}



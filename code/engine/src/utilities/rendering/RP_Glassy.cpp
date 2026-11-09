#include "engine/utilities/rendering/RP_Glassy.h"

#include "engine/Game.h"
#include "engine/components/meshes/Mesh.h"
#include "engine/components/Renderer.h"
#include "engine/components/CubemapRenderer.h"
#include "engine/ResourceManager.h"
#include "engine/utilities/rendering/RenderingUtils.h"
#include "engine/utilities/rendering/Shader.h"
#include "engine/systems/RenderingSystem.h"

namespace EisEngine::rendering {
    using EisEngine::components::Mesh;
    using EisEngine::components::Renderer;
    using EisEngine::components::CubemapRenderer;

    RP_Glassy::RP_Glassy(Game& engine) : RenderingPass(), engine(engine) {
        int width, height;
        glfwGetWindowSize(engine.getWindow(), &width, &height);
        // init FBOs
        InitFBO(0, Vector2((float)width, (float)height));
        InitFBO(1, Vector2((float)width, (float)height));
        RenderingSystem::onResize.addListener([this](const Vector2& v) {
            ResizeFBOItems(v);
        });
    }

    void RP_Glassy::Execute(Camera* camera) {
        // no reflection if no skybox -> abort rendering process
        Entity* skybox = RenderingSystem::GetSkybox();
        if (skybox == nullptr)
            return;

        // shader setup
        Shader* activeShader = ResourceManager::GetShaderOfType(ShaderType::GLASSY);
        // early exit if no renderer is paired with Glassy rendering.
        if (activeShader->IsUnused())
            return;

        // get all renderers associated with glassy rendering.
        glBindVertexArray(VAO);
        std::vector<Renderer*>& assignedRenderers = activeShader->GetSubscribedRenderers();

        // shader switcheroo because needs intermediate passes for depth mapping :]
        // glassy was just used to establish whether shader is in use and what entities it is attached to.
        activeShader = ResourceManager::GetShaderOfType(ShaderType::DEPTH);

        activeShader->Apply(camera);
        // retrieve rendering settings here.
        activeShader->setFloat("ambient", RenderingSystem::GetAmbientLevel());
        activeShader->setFloat("specular", RenderingSystem::GetSpecularFactor());

        DepthMapping(assignedRenderers, activeShader, camera);

        // use actual shader now.
        activeShader = ResourceManager::GetShaderOfType(ShaderType::GLASSY);
        activeShader->Apply(camera);

        ApplyGlassyRenderingSettings(activeShader, skybox);

        for (Renderer* renderer : assignedRenderers) {
            Mesh* mesh = renderer->GetMesh();
            Prepare3DDraw(*mesh, activeShader);
            mesh->draw(activeShader->GetShaderID());
        }

        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }

    void RP_Glassy::DepthMapping(std::vector<Renderer*>& assignedRenderers, Shader* activeShader, Camera* camera) {
        // bind thickness fbo
        // clear color & depth
        // cull front faces
        glBindFramebuffer(GL_FRAMEBUFFER, FBO[0]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);

        for (Renderer* renderer : assignedRenderers) {
            // I think I just need geometry for this one; Edit to fit. <- TO TEST AGAIN!
            // Prepare3DDraw(*mesh, activeShader);
            Mesh* mesh = renderer->GetMesh();
            auto model = mesh->entity()->transform->GetModelMatrix();
            activeShader->setMatrix("mvp", activeShader->CalculateMVPMatrix(model));
            auto view = camera->CalculateViewMatrix();
            activeShader->setMatrix("mv", view * model);
            mesh->draw(activeShader->GetShaderID());
        }

        // redo a second pass for front face.
        glBindFramebuffer(GL_FRAMEBUFFER, FBO[1]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_BACK);

        for (Renderer* renderer : assignedRenderers) {
            // I think I just need geometry for this one; Edit to fit. <- TO TEST AGAIN!
            // Prepare3DDraw(*mesh, activeShader);
            Mesh* mesh = renderer->GetMesh();
            auto model = mesh->entity()->transform->GetModelMatrix();
            activeShader->setMatrix("mvp", activeShader->CalculateMVPMatrix(model));
            auto view = camera->CalculateViewMatrix();
            activeShader->setMatrix("mv", view * model);
            mesh->draw(activeShader->GetShaderID());
        }
    }

    void RP_Glassy::ApplyGlassyRenderingSettings(Shader* activeShader, Entity* skybox) {
        // bind "base" fbo (none)
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        // turn off depth writing
        glDepthMask(GL_FALSE);
        glDisable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        activeShader->setFloat("ambient", RenderingSystem::GetAmbientLevel());
        activeShader->setFloat("specular", RenderingSystem::GetSpecularFactor());

        auto dims = engine.context->GetWindowSize();
        activeShader->setInt("screenWidth", (int)dims.x);
        activeShader->setInt("screenHeight", (int)dims.y);
        activeShader->setVector("eta", RenderingSystem::GetEta());

        // apply cubemap
        CubemapRenderer* renderer = skybox->GetComponent<CubemapRenderer>();
        renderer->ApplyData(*activeShader);

        // bind back depth texture
        glActiveTexture(GL_TEXTURE0 + UniformSamplerIndices::DEPTH_BACK_FACE);
        glBindTexture(GL_TEXTURE_2D, depthTex[0]);
        // bind front depth texture
        glActiveTexture(GL_TEXTURE0 + UniformSamplerIndices::DEPTH_FRONT_FACE);
        glBindTexture(GL_TEXTURE_2D, depthTex[1]);

        GLint boundCube;
        glActiveTexture(GL_TEXTURE0 + UniformSamplerIndices::CUBEMAP);
        glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &boundCube);
        assert(boundCube != 0);
    }

    void RP_Glassy::Prepare3DDraw(Mesh& mesh, Shader* activeShader) {
        auto model = mesh.entity()->transform->GetModelMatrix();
        activeShader->setMatrix("mvp", activeShader->CalculateMVPMatrix(model));
        // model matrices
        activeShader->setMatrix("model", model);
        auto normalMat = glm::mat3(model);
        // if mat is inversible, apply inverse transposed matrix
        normalMat = glm::transpose(glm::inverse(glm::mat3(model)));
        if (abs(glm::determinant(model)) < 1e-6f) {
            // normalize matrix to kill scale variance
            normalMat[0] = glm::normalize(normalMat[0]);
            normalMat[1] = glm::normalize(normalMat[1]);
            normalMat[2] = glm::normalize(normalMat[2]);
        }
        activeShader->setMatrix("normalMat", normalMat);

        // material
        auto renderer = mesh.entity()->GetComponent<Renderer>();
        if (renderer)
            renderer->ApplyData(*activeShader);

        // no lighting effects for now. would be here if there were.
    }

    void RP_Glassy::InitFBO(const int& index, const Vector2& screenDims) {
        // gen framebuffer
        glGenFramebuffers(1, &FBO[index]);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO[index]);

        // gen depth texture
        glGenTextures(1, &depthTex[index]);
        glBindTexture(GL_TEXTURE_2D, depthTex[index]);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_R32F,
            (int)screenDims.x,
            (int)screenDims.y,
            0,
            GL_RED,
            GL_FLOAT,
            nullptr
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D,
            depthTex[index],
            0
        );

        // gen RBO for depth cache (?)
        glGenRenderbuffers(1, &RBO[index]);
        glBindRenderbuffer(GL_RENDERBUFFER, RBO[index]);
        glRenderbufferStorage(
            GL_RENDERBUFFER,
            GL_DEPTH_COMPONENT24,
            (int)screenDims.x,
            (int)screenDims.y
        );

        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_RENDERBUFFER,
            RBO[index]
        );

        // add draw buffer
        GLenum drawBufs[] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, drawBufs);

        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void RP_Glassy::ResizeFBOItems(const EisEngine::Vector2& newScreenDims) {
        for (int i = 0; i < FBO.size(); i++) {
            glBindRenderbuffer(GL_RENDERBUFFER, FBO[i]);

            glBindTexture(GL_TEXTURE_2D, depthTex[i]);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_R32F,
                (int)newScreenDims.x,
                (int)newScreenDims.y,
                0,
                GL_RED,
                GL_FLOAT,
                nullptr
            );

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // gen RBO for depth cache (?)
            glBindRenderbuffer(GL_RENDERBUFFER, RBO[i]);
            glRenderbufferStorage(
                GL_RENDERBUFFER,
                GL_DEPTH_COMPONENT24,
                (int)newScreenDims.x,
                (int)newScreenDims.y
            );

            assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }
}

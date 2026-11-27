#include "engine/utilities/rendering/RP_Glassy.h"

// top-level
#include "engine/ResourceManager.h"
#include "engine/Game.h"
// components
#include "engine/components/meshes/Mesh.h"
#include "engine/components/Renderer.h"
#include "engine/components/CubemapRenderer.h"
// rendering
#include "engine/utilities/rendering/Shader.h"
#include "engine/utilities/rendering/RenderingUtils.h"
// systems
#include "engine/systems/LightSystem.h"
#include "engine/systems/RenderingSystem.h"
#include "glm/gtc/matrix_inverse.hpp"

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

        DepthMapping(assignedRenderers, camera);

        // depth mapping activates a different shader, so only do that sort of stuff now.
        activeShader->Apply();
        AssignCameraValues(activeShader, camera);

        ApplyGlassyRenderingSettings(activeShader, skybox);

        for (Renderer* renderer : assignedRenderers) {
            Mesh* mesh = renderer->GetMesh();

            // if renderer has no associated mesh, skip draw.
            if (mesh == nullptr)
                continue;

            // material
            AssignTransformValues(activeShader, renderer->entity()->transform);
            AssignRendererValues(activeShader, renderer);

            mesh->draw(activeShader->GetShaderID());

            DEBUG_OPENGL("RP_Glassy::Execute/Renderer")
        }

        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }

    void RP_Glassy::AssignCameraValues(Shader* activeShader, Camera* camera) const {
        // camPos for lighting
        GLint camPosLoc = glGetUniformLocation(activeShader->GetShaderID(), "camPos");
        if (camPosLoc != -1)
            activeShader->setVector("camPos", camera->transform->GetGlobalPosition());

        activeShader->setInt("diffMap", UniformSamplerIndices::DIFFUSE);
        activeShader->setInt("nMap", UniformSamplerIndices::NORMAL);
        activeShader->setInt("cubeMap", UniformSamplerIndices::CUBEMAP);
        activeShader->setInt("backDepthMap", UniformSamplerIndices::DEPTH_BACK_FACE);
        activeShader->setInt("frontDepthMap", UniformSamplerIndices::DEPTH_FRONT_FACE);

        DEBUG_OPENGL("RP_Glassy::AssignCameraValues")
    }

    void RP_Glassy::AssignRendererValues(Shader* activeShader, Renderer* renderer) const {
        // material properties
        Material* mat = renderer->material;

        activeShader->setVector("diffuse", mat->GetDiffuse());
        activeShader->setVector("eta", mat->GetEta());
        activeShader->setFloat("alpha", mat->GetOpacity());
        activeShader->setFloat("tiling", mat->GetTiling());
        activeShader->setFloat("F0", mat->GetSpecular());
        activeShader->setFloat("roughness", mat->GetRoughness());
        activeShader->setFloat("absorption", mat->GetAbsorption());

        renderer->ApplyTextureOfType(activeShader, UniformSamplerIndices::DIFFUSE, "diffMap");
        renderer->ApplyTextureOfType(activeShader, UniformSamplerIndices::NORMAL, "nMap");

        DEBUG_OPENGL("RP_Glassy::AssignRendererValues")
    }

    void RP_Glassy::AssignTransformValues(Shader* activeShader, Transform* transform) const {
        Vector3 pos = transform->GetGlobalPosition();

        glm::mat4 model = transform->GetModelMatrix();
        activeShader->setMatrix("mvp", activeShader->CalculateMVPMatrix(model));
        // model matrices
        activeShader->setMatrix("model", model);

        // normal matrix:
        glm::mat3 model3x3 = glm::mat3(model);

        // check inversibility to guard against singular matrices (NaN values if inverted)
        // [GenAI Disclosure] logic for this check entirely written by Claude Sonnet 5.5
        float scaleProduct = glm::length(model3x3[0]) * glm::length(model3x3[1]) * glm::length(model3x3[2]);
        float det = glm::determinant(model3x3);

        // if mat is inversible, apply inverse transposed matrix (det != 0; scaled by total magnitude of m3x3 for cases of |m| < 1e-6f) else just m3x3
        glm::mat3 normalMat = (scaleProduct > 0 && std::abs(det) > 1e-6f * scaleProduct) ?
            glm::inverseTranspose(glm::mat3(model3x3)) :
            model3x3;

        activeShader->setMatrix("normalMat", normalMat);

        DEBUG_OPENGL("RP_Glassy::AssignTransformValues")
    }

    void RP_Glassy::DepthMapping(std::vector<Renderer*>& assignedRenderers, Camera* camera) {

        // shader switcheroo because needs intermediate passes for depth mapping :]
        // glassy was just used to establish whether shader is in use and what entities it is attached to.
        Shader* activeShader = ResourceManager::GetShaderOfType(ShaderType::DEPTH);

        activeShader->Apply();

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
        // reenable all the settings disabled for depth mapping.
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        // turn off depth writing
        glDepthMask(GL_FALSE);
        glDisable(GL_CULL_FACE);
        glCullFace(GL_BACK);


        Vector2 dims = engine.context->GetWindowSize();
        activeShader->setInt("screenWidth", (int)dims.x);
        activeShader->setInt("screenHeight", (int)dims.y);

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

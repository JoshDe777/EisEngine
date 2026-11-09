#include "engine/utilities/rendering/RP_BlinnPhong.h"

// top-level
#include "engine/ResourceManager.h"
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

namespace EisEngine::rendering {
    using EisEngine::components::CubemapRenderer;
    using EisEngine::components::Renderer;
    using EisEngine::rendering::Shader;
    using EisEngine::ResourceManager;
    using EisEngine::rendering::ShaderType;
    using EisEngine::components::Mesh;
    using EisEngine::systems::LightSystem;

    void RP_BlinnPhong::Execute(Camera* camera) {
        Shader* activeShader = ResourceManager::GetShaderOfType(ShaderType::SKYBOX);
        // early exit if no renderer is paired with Blinn-Phong rendering.
        if (activeShader->IsUnused())
            return;

        glBindVertexArray(VAO);
        activeShader->Apply(camera);

        // exactly one skybox at any time, so no looping. 
        // Will always use the first skybox in the SubscribedRenderers list for the shader.
        Entity* skybox = activeShader->GetSubscribedRenderers()[0]->GetMesh();

        glDisable(GL_CULL_FACE);

        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        auto view = glm::mat4(glm::mat3(camera->CalculateViewMatrix()));
        auto proj = camera->GetProjectionMatrix();

        activeShader->setMatrix("vp", proj * view);

        // apply cubemap
        CubemapRenderer* renderer = skybox->GetComponent<CubemapRenderer>();
        renderer->ApplyData(*activeShader);

        Mesh* mesh = skybox->GetMesh();
        mesh->draw(activeShader->GetShaderID());

        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);
    }
}
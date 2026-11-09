#include "engine/utilities/rendering/RP_BlinnPhong.h"

// top-level
#include "engine/ResourceManager.h"
// components
#include "engine/components/meshes/Mesh.h"
#include "engine/components/Renderer.h"
// rendering
#include "engine/utilities/rendering/Shader.h"
#include "engine/utilities/rendering/RenderingUtils.h"
// systems
#include "engine/systems/LightSystem.h"
#include "engine/systems/RenderingSystem.h"

namespace EisEngine::rendering {
    using EisEngine::rendering::Shader;
    using EisEngine::ResourceManager;
    using EisEngine::rendering::ShaderType;
    using EisEngine::components::Mesh;
    using EisEngine::systems::LightSystem;

	void RP_BlinnPhong::Execute(Camera* camera) {

        Shader* activeShader = ResourceManager::GetShaderOfType(ShaderType::BLINN_PHONG);
        // early exit if no renderer is paired with Blinn-Phong rendering.
        if (activeShader->IsUnused())
            return;  

        glBindVertexArray(VAO);
        activeShader->Apply(camera);
        activeShader->setFloat("ambient", RenderingSystem::GetAmbientLevel());
        activeShader->setFloat("specular", RenderingSystem::GetSpecularFactor());

        std::vector<Renderer*>& assignedRenderers = activeShader->GetSubscribedRenderers();
        for (Renderer*& renderer : assignedRenderers) {
            Mesh* mesh = renderer->GetMesh();
            // if renderer has no associated mesh, skip.
            if (mesh == nullptr)
                continue;

            Vector3 pos = mesh->entity()->transform->GetGlobalPosition();

            glm::mat4 model = mesh->entity()->transform->GetModelMatrix();
            activeShader->setMatrix("mvp", activeShader->CalculateMVPMatrix(model));
            // model matrices
            activeShader->setMatrix("model", model);
            glm::mat3 normalMat = glm::mat3(model);

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
            renderer->ApplyData(*activeShader);

            // lighting - compute light cuts & apply to shader
            // pass the light system reference into the constructor.
            LightSystem::ComputeAndApplyLightCuts(
                pos,
                RenderingSystem::GetLODDist(pos),
                activeShader);

            mesh->draw(activeShader->GetShaderID());
        }
	}
}
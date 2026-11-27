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
#include "glm/gtc/matrix_inverse.hpp"

namespace EisEngine::rendering {
    using EisEngine::rendering::Shader;
    using EisEngine::ResourceManager;
    using EisEngine::rendering::ShaderType;
    using EisEngine::components::Mesh;
    using EisEngine::systems::LightSystem;

    void RP_BlinnPhong::AssignCameraValues(Shader* activeShader, Camera* camera) const {
        // camPos for lighting
        GLint camPosLoc = glGetUniformLocation(activeShader->GetShaderID(), "camPos");
        if (camPosLoc != -1)
            activeShader->setVector("camPos", camera->transform->GetGlobalPosition());

        activeShader->setInt("diffMap", UniformSamplerIndices::DIFFUSE);
        activeShader->setInt("nMap", UniformSamplerIndices::NORMAL);

        DEBUG_OPENGL("RP_BlinnPhong::AssignCameraValues")
    }

    void RP_BlinnPhong::AssignRendererValues(Shader* activeShader, Renderer* renderer) const {
        // material properties
        Material* mat = renderer->material;

        activeShader->setVector("diffuse", mat->GetDiffuse());
        activeShader->setFloat("alpha", mat->GetOpacity());
        activeShader->setFloat("tiling", mat->GetTiling());
        activeShader->setFloat("roughness", mat->GetRoughness());
        activeShader->setFloat("specular", mat->GetSpecular());

        renderer->ApplyTextureOfType(activeShader, UniformSamplerIndices::DIFFUSE, "diffMap");
        renderer->ApplyTextureOfType(activeShader, UniformSamplerIndices::NORMAL, "nMap");

        DEBUG_OPENGL("RP_BlinnPhong::AssignRendererValues")
    }

    void RP_BlinnPhong::AssignTransformValues(Shader* activeShader, Transform* transform) const {
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

        DEBUG_OPENGL("RP_BlinnPhong::AssignTransformValues")
    }

	void RP_BlinnPhong::Execute(Camera* camera) {
        Shader* activeShader = ResourceManager::GetShaderOfType(ShaderType::BLINN_PHONG);
        // early exit if no renderer is paired with Blinn-Phong rendering.
        if (activeShader->IsUnused())
            return;

        // apply global properties
        glBindVertexArray(VAO); 
        activeShader->Apply();
        AssignCameraValues(activeShader, camera);
        activeShader->setFloat("ambient", RenderingSystem::GetAmbientLevel());
        activeShader->setFloat("maxShininess", RenderingSystem::GetMaxShininess());

        std::vector<Renderer*>& assignedRenderers = activeShader->GetSubscribedRenderers();
        for (Renderer*& renderer : assignedRenderers) {
            Mesh* mesh = renderer->GetMesh();

            // if renderer has no associated mesh, skip draw.
            if (mesh == nullptr)
                continue;

            // material
            AssignTransformValues(activeShader, renderer->entity()->transform);
            AssignRendererValues(activeShader, renderer);

            Vector3 pos = renderer->entity()->transform->GetGlobalPosition();

            // lighting - compute light cuts & apply to shader
            RenderingSystem::GetLightSystem()->ApproximateLighting(pos, RenderingSystem::GetLODDist(pos), activeShader);

            mesh->draw(activeShader->GetShaderID());

            DEBUG_OPENGL("RP_BlinnPhong::Execute/Renderer")
        }
	}
}
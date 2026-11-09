#include "engine/utilities/rendering/RP_ScreenSpaceUI.h"

#include "engine/components/meshes/Mesh.h"
#include "engine/systems/RenderingSystem.h"
#include "engine/utilities/rendering/Shader.h"

namespace EisEngine::rendering {
    using EisEngine::rendering::Shader;
    using EisEngine::components::Mesh;
	void RP_ScreenSpaceUI::Execute(Camera* camera) {
        
        Shader* activeShader = ResourceManager::GetShaderOfType(ShaderType::SCREEN_UI);
        // early exit if no renderer is paired with Blinn-Phong rendering.
        if (activeShader->IsUnused())
            return;

        // disable depth testing here for UI
        glDisable(GL_DEPTH_TEST);

        std::vector<Renderer*>& assignedRenderers = activeShader->GetSubscribedRenderers();

        // sort by ascending z values for layering because no depth test.
        std::sort(assignedRenderers.begin(), assignedRenderers.end(), CompareZValues);

        glBindVertexArray(VAO);
        activeShader->Apply(camera);

        // for every renderer associated with this shader:
        for (Renderer* renderer : assignedRenderers) {
            Mesh* mesh = renderer->GetMesh();
            // if renderer has no associated mesh, skip.
            if (mesh == nullptr)
                continue;

            renderer->ApplyData(*activeShader);
            int screenWidth = camera->GetWidth();
            int screenHeight = camera->GetHeight();

            // project sprite orthographically in 2D.
            glm::mat4 projection = glm::ortho(-(float)screenWidth / 2, (float)screenWidth / 2,
                -(float)screenHeight / 2, (float)screenHeight / 2);
            // no view matrix since coordinates assumed already in screen space!
            glm::mat4 modelProjection = projection * mesh->entity()->transform->GetModelMatrix();
            activeShader->setMatrix("mvp", modelProjection);
            mesh->draw(activeShader->GetShaderID());
        }
        
        // reenable depth testing (default val) for next shader.
        glEnable(GL_DEPTH_TEST);
	}

	bool RP_ScreenSpaceUI::CompareZValues(Renderer* a, Renderer* b) {
		return a->entity()->transform->GetGlobalPosition().z < b->entity()->transform->GetGlobalPosition().z;
	}
}

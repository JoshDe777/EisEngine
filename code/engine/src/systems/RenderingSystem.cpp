#include "engine/systems/RenderingSystem.h"
#include "engine/Game.h"
#include "engine/Components.h"

#include <algorithm>

namespace EisEngine::systems {
    // static member assignment:

    Entity* RenderingSystem::skybox = nullptr;
    Event<RenderingSystem, const Vector2&> RenderingSystem::onResize = Event();
    std::vector<Entity*> RenderingSystem::Loaders = {};
    RenderingConfig& RenderingSystem::config = RenderingConfig::GetInstance();
    LightSystem* RenderingSystem::lightSystem = nullptr;

    // rendering system constructor
    RenderingSystem::RenderingSystem(EisEngine::Game &engine) : System(engine) {
        camera = engine.camera.get();
        if(!camera)
            DEBUG_RUNTIME_ERROR("Cannot initialize rendering; Camera not found.")

        if(lightSystem == nullptr)
            lightSystem = engine.lightSystem.get();

        engine.onUpdate.addListener([&] (Game& engine){ Draw();});
        #pragma endregion

        // add callback to window resize to reallocate depth texture size & rbo sizes on window resize.
        glfwSetWindowSizeCallback(engine.getWindow(), [](GLFWwindow* window, int width, int height){
            RenderingSystem::onResize.invoke(Vector2((float) width, (float) height));
        });

        glDisable(GL_CULL_FACE);
    }

    void RenderingSystem::MarkAsLoader(EisEngine::ecs::Entity *ptr) {
        Loaders.push_back(ptr);
    }

    void RenderingSystem::SetSkyboxEntity(EisEngine::ecs::Entity *ptr) {
        skybox = ptr;
    }

    void RenderingSystem::Draw() {
        // re-enable depth testing for 'regular' entities. (just in case)
        glEnable(GL_DEPTH_TEST);
        
        // cycle through RenderPass objects here and execute them.
    }

    float RenderingSystem::GetLODDist(const Vector3& pos) {
        float lodDist = 100000000000000000.0f;
        // base LOD --> if within distance of loader object, render normally else render ambiant
        if (!Loaders.empty()) {
            for (Entity* obj : Loaders) {
                Vector3 objPos = obj->transform->GetGlobalPosition();
                lodDist = std::min(lodDist, Vector3::Distance(objPos, pos));
            }
        }
        else {
            lodDist = 0.0f;    // return highest possible LOD value (no LOD) if no objects were labelled as loaders.
        }

        return lodDist;
    }
}

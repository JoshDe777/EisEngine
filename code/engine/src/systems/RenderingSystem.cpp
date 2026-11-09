#include "engine/systems/RenderingSystem.h"
#include "engine/Game.h"
#include "engine/Components.h"

#include <algorithm>

// DO NOT UPDATE WITHOUT ALSO UPDATING SAME NAMED MACRO IN FRAGMENT SHADERS!
#define MAX_LIGHTS 5
#define LOD_DIST_THRESHOLD 10.0f

namespace EisEngine::systems {

    // specular factor starts at 1
    float RenderingSystem::specularFactor = 1.0f;
    // toon levels default to 8
    int RenderingSystem::n_toon_levels = 8;
    // no chromatic aberration
    Vector3 RenderingSystem::eta = Vector3::zero;
    // 15% ambiant lighting
    float RenderingSystem::ambient = 0.15f;
    // no skybox by default. Pitch black background.
    Entity* RenderingSystem::skybox = nullptr;
    // Event called every time the window is resized - for resizing framebuffers for example.
    Event<RenderingSystem, const Vector2&> RenderingSystem::onResize = Event();

    struct LightEntry{
        PointLight* L;
        float dist2;
    };

    std::vector<Entity*> RenderingSystem::Loaders = {};

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

    // rendering system constructor
    RenderingSystem::RenderingSystem(EisEngine::Game &engine) : System(engine) {
        camera = engine.camera.get();
        if(!camera)
            DEBUG_RUNTIME_ERROR("Cannot initialize rendering; Camera not found.")

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
}

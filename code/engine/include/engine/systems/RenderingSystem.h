#pragma once

#include <OpenGL/OpenGlInclude.h>

#include "engine/systems/LightSystem.h"
#include "engine/ecs/System.h"
#include "engine/systems/Camera.h"
#include "engine/utilities/rendering/Shader.h"
#include "engine/utilities/rendering/RenderingUtils.h"

namespace EisEngine{
    namespace events{
        template<typename Owner, typename ...Args>
        class Event;
    }

    namespace systems {
        using LightSystem = EisEngine::systems::LightSystem;
        using namespace rendering;

        /// The system drawing objects onto the display.
        class RenderingSystem : public System {
            using Event = EisEngine::events::Event<RenderingSystem, const Vector2&>;
        public:
            /// creates an instance of the EisEngine rendering system.
            explicit RenderingSystem(Game& engine);
            /// displays meshes of all supported kinds on screen.
            void Draw();

            /// Adds an entity as an LOD loader object.
            static void MarkAsLoader(Entity* ptr);
            /// Sets the entity to be displayed as the skybox in the scene.
            static void SetSkyboxEntity(Entity* ptr);

            /// sets the amount of brightness levels for the toon shader.
            static void SetToonLevelCount(const int& n) { config.SetToonLevelCount(n); }
            /// sets the base lighting level for a scene.
            static void SetAmbientLevel(const float& val) { config.SetAmbientFactor(val); }

            /// Returns the amount of light levels supported for cartoon-style renders.
            static int GetToonLevelCount() { return config.GetToonLevelCount(); }
            /// Returns the level of ambiant light as a percentage of maximum lighting [0, 1].
            static float GetAmbientLevel() { return config.GetAmbientFactor(); }
            /// Returns the maximum shiny exponent value for Blinn-Phong-style specular approximations.
            static float GetMaxShininess() { return config.GetMaxShininess(); }
            /// Establishes how far an entity is from the nearest loader object.
            static float GetLODDist(const Vector3& pos);
            /// Get a pointer to the entity currently serving as a skybox.
            static Entity* GetSkybox() { return skybox; }
            /// An event called every time the window resizes.
            static Event onResize;

            static LightSystem* GetLightSystem() { return lightSystem; }

        private:
            // composite members
            /// A pointer to the active camera object.
            Camera* camera = nullptr;
            /// A pointer to the game's light system object.
            static LightSystem* lightSystem;
            /// A reference to the current rendering configuration
            static RenderingConfig& config;
            /// A list of entities enabling other entities in a certain radius of them to be lit.
            static std::vector<Entity*> Loaders;
            /// A pointer to the entity marked as a skybox.
            static Entity* skybox;
        };
    }
}

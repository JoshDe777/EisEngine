#pragma once

#include <OpenGL/OpenGlInclude.h>
#include "engine/systems/LightSystem.h"
#include "engine/ecs/System.h"
#include "engine/systems/Camera.h"
#include "engine/utilities/rendering/Shader.h"

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

            /// changes the specular factor in the scene for the Blinn-Phong Shader.
            static void SetSpecularFactor(const float& val) { specularFactor = val; }
            /// sets the amount of brightness levels for the toon shader.
            static void SetToonLevelCount(const int& n) {n_toon_levels = n;}
            /// sets the base lighting level for a scene.
            static void SetAmbientLevel(const float& val) { ambient = val;}
            /// sets the level of chromatic aberration. Unused outside of glassy shader.
            static void SetEta(const Vector3& val) { eta = val;}

            /// Returns the specular factor for the current renderer.
            static float GetSpecularFactor() { return specularFactor; }
            /// Returns the amount of light levels supported for cartoon-style renders.
            static int GetToonLevelCount() { return n_toon_levels; }
            /// Returns the level of ambiant light as a percentage of maximum lighting [0, 1].
            static float GetAmbientLevel() { return ambient; }
            /// Returns the Eta vector representing chromatic aberration on glassy surfaces in RGB-spaces.
            static Vector3 GetEta() { return eta; }
            /// Establishes how far an entity is from the nearest loader object.
            static float GetLODDist(const Vector3& pos);
            /// Get a pointer to the entity currently serving as a skybox.
            static Entity* GetSkybox() { return skybox; }
            /// An event called every time the window resizes.
            static Event onResize;

        private:
            // composite members
            /// A pointer to the active camera object.
            Camera* camera = nullptr;
            /// A pointer to the game's light system object.
            LightSystem* lightSystem = nullptr;
            /// A list of entities enabling other entities in a certain radius of them to be lit.
            static std::vector<Entity*> Loaders;
            /// A pointer to the entity marked as a skybox.
            static Entity* skybox;

            // rendering parameters
            /// [Blinn-Phong] The specular factor determining how sharp the specular lobe is.\n
            /// The higher this value, the slimmer the lobe.
            static float specularFactor;
            /// the amount of brightness levels available to the toon shader.
            static int n_toon_levels;
            /// Introduced as a chromatic aberration factor. Unused outside of glassy shader.
            static Vector3 eta;
            /// Proportion of ambient lighting.\n
            /// ambient = 1 -> fully lit scene.
            static float ambient;
        };
    }
}

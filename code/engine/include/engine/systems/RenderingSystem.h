#pragma once

#include <OpenGL/OpenGlInclude.h>
#include "engine/systems/LightSystem.h"
#include "engine/ecs/System.h"
#include "Camera.h"
#include "engine/utilities/rendering/Shader.h"

namespace EisEngine{
    namespace components{
        class Mesh2D;
        class Line;
        class Mesh3D;
        class SpriteMesh;
    }
    namespace events{
        template<typename Owner, typename ...Args>
        class Event;
    }
    namespace systems {
        using LightSystem = EisEngine::systems::LightSystem;
        using LightCluster = EisEngine::systems::LightCluster;

        struct ShaderLightStruct{
            glm::vec3 pos;
            float _padding;
            glm::vec3 em;
            float I;
        };

        using namespace rendering;
        /// The system drawing objects onto the display.
        class RenderingSystem : public System {
            using Mesh3D = EisEngine::components::Mesh3D;
            using Mesh2D = EisEngine::components::Mesh2D;
            using Line = EisEngine::components::Line;
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

            /// Switches the shader for 3D objects to the requested shader.
            static void SetActiveShader(const std::string& shaderName);
            /// changes the specular factor in the scene for the Blinn-Phong Shader.
            static void SetSpecularFactor(const float& val);
            /// sets the amount of brightness levels for the toon shader.
            static void SetToonLevelCount(const int& n) {n_toon_levels = n;}
            /// sets the base lighting level for a scene.
            static void SetAmbientLevel(const float& val) { ambient = val;}
            /// sets the level of chromatic aberration. Unused outside of glassy shader.
            static void SetEta(const Vector3& val) { eta = val;}

            /// calculates the maximum possible value for an effecting light.
            static float GetMaxBRDF(const std::string& brdfFunc = "Blinn-Phong");
            /// [DEPRECATING] Sets whether to calculate lighting based on voxel nearest-neighbours, or light-cut global approximation. 
            static void SetLightMode(bool& _useVoxelGrid) {useVoxelGrid = _useVoxelGrid;}
        private:
            // FBO functions
            /// Initializes the framebuffer object for depth mapping.
            void InitFBO(const int& index, const Vector2& screenDims);
            /// Resizes buffers on window size shift.
            void ResizeFBOItems(const Vector2& newScreenDims);

            // draw sub-functions
            /// Draws 2D meshes
            static void DrawMesh2D(Mesh2D& mesh, Shader* activeShader);
            /// Draws Lines
            static void DrawLine(Line& mesh, Shader* activeShader);
            /// Draws the skybox if it exists
            void DrawSkybox(Shader* activeShader);
            /// Prepares shaders to draw a 3D Mesh
            void Prepare3DDraw(Mesh3D& mesh, Shader* activeShader);
            /// Returns a list of light sources retrieved from the voxel entourage.
            void GetGridLights(Mesh3D& mesh, Shader* activeShader);
            /// Returns a list of light clusters retrieved from the barnes hut approximation.
            void GetLightClusters(Mesh3D& mesh, Shader* activeShader);
            /// Takes in a list of light cluster results and applies them to the shader.
            void ApplyLightEntriesToShader(std::vector<ShaderLightStruct>& results, Shader* activeShader) const;
            /// Drawing program for all translucent objects.
            void DrawTransparentObjects(std::vector<Mesh3D*>& transparentMeshes,
                                        Shader* activeShader);

            // shader stuff
            /// VAO array storing a VAO for each type of mesh in order: Mesh2D, Line, Mesh3D, SpriteMesh, uiMesh.
            std::array<GLuint, 5> VAO;
            /// FBO array storing a depth-mapping FBO.
            std::array<GLuint, 2> FBO;
            /// FBO array storing a depth-mapping RBO.
            std::array<GLuint, 2> RBO;
            /// SSBO object used for passing light cluster data to the shader.
            GLuint lightCutSSBO;
            /// Texture index storing depth data.
            std::array<GLuint, 2> depthTex;
            /// The name (human-readable) of the default shader for the rendering system.\n
            /// Must be a key value for a shaderNameDict entry.
            static const std::string defaultShader;
            /// The name (human-readable) of the currently active 3D shader for the rendering system.\n
            /// Must be a key value for a shaderNameDict entry.
            static std::string active3DShader;
            /// A dictionary linking shader names to the name of their corresponding shader object in the ResourceManager.
            static const std::unordered_map<std::string, std::string> shaderNameDict;

            // composite members
            /// A pointer to the active camera object.
            Camera* camera = nullptr;
            /// A pointer to the game's light system object.
            LightSystem* lightSystem = nullptr;
            /// A list of entities enabling other entities in a certain radius of them to be lit.
            static std::vector<Entity*> Loaders;
            /// A pointer to the entity marked as a skybox.
            static Entity* skybox;
            /// An event called every time the window resizes.
            static Event onResize;

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
            /// Determines whether lighting is calculated using voxel-based NN or light-cut global approximation.
            static bool useVoxelGrid;
        };
    }
}

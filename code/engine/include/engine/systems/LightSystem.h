#pragma once

#include "engine/ecs/System.h"
#include "engine/utilities/Vector3.h"
#include "engine/utilities/rendering/Material.h"
#include "engine/utilities/rendering/LightCluster.h"
#include "engine/components/PointLight.h"

#include <utility>
#include <vector>
#include <unordered_map>
#include <memory>

namespace EisEngine {
    // forward declarations :)
    class Game;
    namespace components{ class Mesh3D;}
    namespace ecs { class Entity;}

    namespace systems {
        using PointLight = EisEngine::components::PointLight;

        class RenderingSystem;

        /// A hash map, 3D voxel-based Spatial Data Structure (=SDS) for storing light sources.
        struct GridCoordHashMap {
            size_t operator()(const Vector3 &c) const {
                // hash numbers from "Optimized spatial hashing for collision detection of
                // deformable objects" - Teschner et.al., 2003.
                // additive instead of XOR for added collision-robustness - values suggested by ChatGPT.
                return (std::hash<int>()((int) c.x) * 73856093) +
                       (std::hash<int>()((int) c.y) * 19349663) +
                       (std::hash<int>()((int) c.z) * 83492791);
            }
        };

        /// Stores and manages light sources and estimation in the active scene.
        class LightSystem : public EisEngine::ecs::System {
            friend class RenderingSystem;
            using Entity = EisEngine::ecs::Entity;
            using LightCluster = EisEngine::systems::LightCluster;
        public:
            /// creates an instance of the EisEngine light system.
            explicit LightSystem(Game& engine);

            /// (Deterministic Barnes-Hut) Replaces obsolete QueryNearbyLights.
            /// Computes a list of light clusters that provide an adequately accurate lighting calculation for the given position.
            /// @param pos: Vector3 - the position in world space for the light approximation.
            /// @param LODDist: float - the distance in world space to the nearest loader object. The smaller the distance, the more accurate the lighting approximation.
            std::vector<LightCluster*> ComputeLightCut(
                    Vector3& pos,
                    const float& LODDist
                ) const;

            /// (Voxel grid) [OBSOLETE - use ComputeLightCut() instead!] Checks the surroundings of an object for effecting light sources.
            std::vector<int> QueryNearbyLights(const glm::vec3 &objectPos);

            /// (Voxel grid) Signals to the light system that an emitting entity has changed position.
            static void MarkLightForUpdate(const int& entityID);

            /// (Voxel grid) Registers a light source at the given world position.\n
            /// If said light source is already in the grid, it's old reference is removed.
            void InsertEntityAt(const int& entityID, const Vector3& pos);
            /// Removes an entity from the light SDS
            void RemoveEntity(const int& entityID);
            /// (Voxel grid) 3D cell size for light source voxel size in storage; (CELL_SIZE x CELL_SIZE x CELL_SIZE).
            static constexpr float CELL_SIZE = 5.0f;

            /// Sets the maximum accuracy accepted as valid for light cut calculation. 
            /// The lower the threshold, the more accurate calculation will be, at the cost of performance.
            /// @param val: float - the new threshold value.
            static void SetBaseBHThreshold(const float& val) { BASE_THRESHOLD = val;}
            /// Sets the threshold steepness value for light cut calculation.
            /// Steepness controls how fast the accuracy can fall off from maximum accuracy (close to a loader object & specific light source)
            /// @param val: float - the new steepness value. 
            static void SetBHThresholdSteepness(const float& val) { ERROR_STEEPNESS = val;}
            /// Sets the threshold stretch value for light cut calculation.
            /// Stretch controls how the accuracy evens out, the further an object is from maximum accuracy 
            /// (stretch = 0 => there is no stabilization of the accuracy, stretch -> 1 => Accuracy evens out almost immediately)
            /// @param val: float - the new stretch value. 
            static void SetBHThresholdStretch(const float& val) { ERROR_STRETCH = val;}
        private:
            /// (Voxel grid) A lookup table for lights' world positions, to determine whether an update is needed.
            std::unordered_map<int, Vector3> lastKnownWorldPos = {};
            /// (Voxel grid) A reference of Point Lights by approximate position in world 3D (x, y, z) space.
            std::unordered_map<Vector3, std::vector<int>, GridCoordHashMap> LightGrid = {};
            /// (Voxel grid) A reverse-reference of entity IDs to their bounding voxel.
            std::unordered_map<int, Vector3> entityGridPos = {};
            /// The light cluster computed to represent the root of the SDS tree.
            std::unique_ptr<LightCluster> root = nullptr;
            /// A lookup table mapping each entity to the light cluster it is located at.
            std::unordered_map<int, LightCluster*> entityTreePos = {};

            /// (Voxel grid) Find the voxel a given entity is in.
            /// Returns (NaN, NaN, NaN) if the entity is not in the grid, please check against it!
            /// -> std::isnan(result.x) == True if invalid.
            Vector3 FindEntityVoxel(const int& entityID);

            /// Updates any light sources that have moved since the last frame.
            void UpdateLightSDS();
            /// (Voxel grid) moves the entity from its current to its new voxel.
            void UpdateInGrid(Entity *entity);
            /// (Light cuts) currently marks the tree for a complete rebuild. Plans to make it simply prune the branch it is in, and reinsert the light source elsewhere.
            void UpdateInBHTree(Entity* entity);
            /// (Voxel grid) inserts a new light source to the grid.
            void InsertEntityToGrid(const int& entityID, const Vector3& pos);
            /// (Light cuts) inserts a new light source to the tree.
            void InsertEntityToBHTree(const int& entityID, const Vector3& pos);
            /// Builds a Barnes-Hut tree with all light sources in scene.
            std::unique_ptr<LightCluster> BuildBHTree();
            /// Recursive sub-tree builder for a section of light sources.
            std::unique_ptr<LightCluster> BuildBalancedTree(
                    std::vector<PointLight*>& lights,
                    int start,
                    int end);
            /// (Voxel grid) Deregisters a light source from the light grid.
            void RemoveEntityFromGrid(const int& entityID);
            /// (Light Cuts) Prunes a branch from the tree.
            void RemoveClusterFromBHTree(LightCluster* cluster);

            /// Calculates the light accuracy value in one dimension.
            static float threshold1D(const float& dist);
            /// Calculates the light accuracy threshold value.
            static float thresholdFunc(const float& lodDist, const float& clusterDist);

            /// (Voxel grid) A static reference list of entities whose light sources might need an updated voxel placement.
            static std::vector<unsigned int> lightsToUpdate;

            /// Lighting parameter designating how fast lighting accuracy may deteriorate when going further from maximum accuracy requirements.
            /// The higher this value, the sharper the decline in accuracy around loader objects.
            /// This value MUST BE NON-ZERO!
            static float ERROR_STEEPNESS;
            /// Lighting parameter designating how fast, and how low lighting accuracy stabilizes the further you go from maximum accuracy requirements.
            /// The higher this value, the faster accuracy thresholds stabilize, and the higher the overall required accuracy around the scene.
            static float ERROR_STRETCH;
            /// Lighting parameter designating an offset for lighting accuracy requirements. 
            /// The higher this value, the lower accuracy is required to consider lighting approximation adequate.
            static float BASE_THRESHOLD;
        };
    }
}

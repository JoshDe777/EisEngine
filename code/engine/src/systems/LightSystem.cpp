#include "engine/Game.h"
#include "engine/components/meshes/Mesh3D.h"
#include "engine/systems/LightSystem.h"

#include <limits>
#include <cmath>
#include <queue>

namespace EisEngine::systems {

#pragma region non-class utilities:
    inline Vector3 WorldToCell(const glm::vec3& pos) {
        return Vector3{
                floor(pos.x / LightSystem::CELL_SIZE),
                floor(pos.y / LightSystem::CELL_SIZE),
                floor(pos.z / LightSystem::CELL_SIZE)
        };
    }

// tuned by hand with some trial and error.
    float LightSystem::ERROR_STEEPNESS = 0.075f;
    float LightSystem::ERROR_STRETCH = 0.001f;
    float LightSystem::BASE_THRESHOLD = 0.1f;
#pragma endregion

#pragma region constructors & static members
    // static list of lights to update, to avoid requiring a reference to the LS
    std::vector<unsigned int> LightSystem::lightsToUpdate = {};

    // constructor
    LightSystem::LightSystem(EisEngine::Game &engine) : System(engine) {
        // on before draw -> update light reference (dynamic 3D SDS)
        engine.onAfterUpdate.addListener([&](Game& engine){
            UpdateLightSDS();
        });

        // init SSBO
        glGenBuffers(1, &lightCutSSBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightCutSSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }
#pragma endregion

#pragma region light update handling
    // marks an entity's light source for update.
    void LightSystem::MarkLightForUpdate(const int &entityID) {
        lightsToUpdate.push_back(entityID);
    }

    // introduce a new light source at a given position.
    void LightSystem::InsertEntityAt(const int &entityID, const Vector3 &pos) {
        lastKnownWorldPos.insert({entityID, pos});
        InsertEntityToBHTree(entityID, pos);
    }

    // remove an entity as a light source.
    void LightSystem::RemoveEntity(const int &entityID) {
        auto cluster = entityTreePos.at(entityID);
        if (cluster != nullptr) {
            RemoveClusterFromBHTree(cluster);
            entityTreePos.erase(entityID);
        }
    }
#pragma endregion

#pragma region dynamic SDS stuff
    void LightSystem::UpdateLightSDS() {
        // no updates to make if no lights in scene or to update
        if(lightsToUpdate.empty() || !engine.componentManager->hasComponentOfType<PointLight>())
            return;

        // check whether the light source underwent a voxel change. If so, rebuild the entire B-H tree as well.
        bool updateTree = false;
        for(auto id : lightsToUpdate){
            auto entity = engine.entityManager->getEntity((int) id);

            auto pos = entity->transform->GetGlobalPosition();

            // bypass distance check if newly registered
            auto it = lastKnownWorldPos.find((int) id);
            if(it != lastKnownWorldPos.end()){
                auto& last = lastKnownWorldPos.at((int) id);

                // don't update if not within voxel change dist of previously known pos.
                if (Vector3::Distance(last, pos) < CELL_SIZE) {
                    continue;
                }
            }

            // if changed voxel mark tree for rebuild (expensive, I know)
            updateTree = true;
            lastKnownWorldPos[(int) id] = pos;
        }

        // rebuild only once
        if(updateTree)
            root = BuildBHTree();

        // reset the lightsToUpdate lookup :D!
        lightsToUpdate.clear();
    }

    void LightSystem::UpdateInBHTree(EisEngine::systems::LightSystem::Entity *entity) {
        // function under construction. This attempt doesn't work at the minute!
        // Current state, the tree just adds the point light to the highest available cluster, making it an entirely one-sided tree.

        // try looking up the closest light source - O(n) + then add it to the tree by simply 
        // substituting that branch, then propagate up the bounding box & intensity updates?
        auto light = entity->GetComponent<PointLight>();
        if(light == nullptr)
            return;

        // find light in BHTree
        auto it = entityTreePos.find(entity->guid());
        if (it != entityTreePos.end()){
            // if light is already in the tree
            auto cluster = entityTreePos.at(entity->guid());

            // if position hasn't changed, return
            bool pos_changed = light->position() != cluster->bounding_box.Centre();
            if(!pos_changed)
                return;

            // otherwise prune it from the tree.
            RemoveClusterFromBHTree(cluster);
            entityTreePos.erase(entity->guid());
        }

        // (re-)insert light source somewhere else.
        InsertEntityToBHTree(entity->guid(), light->position());
    }

#pragma endregion

#pragma region barnes-hut stuff
    float LightSystem::threshold1D(const float& dist){
        // gives the accuracy threshold value in one dimension (distance to drawn entity OR distance to loader entity)
        // function designed for control over pace of accuracy loss [ERROR_STEEPNESS] 
        // and accuracy stabilization at greater distance [ERROR_STRETCH]
        return (float) (ERROR_STEEPNESS * pow(dist, 2.0f)) / (1 + ERROR_STRETCH * pow(dist, 2));
    }

    float LightSystem::thresholdFunc(const float& lodDist, const float& clusterDist){
        // gives the accuracy threshold value in two dimensions (distance to drawn entity AND distance to loader entity)
        // function designed for control over pace of accuracy loss [ERROR_STEEPNESS],
        // and accuracy stabilization at greater distance [ERROR_STRETCH],
        // + a direct offset [BASE_THRESHOLD].
        return BASE_THRESHOLD + threshold1D(lodDist) + threshold1D(clusterDist);
    }

    // custom comparison metric for clusters to ensure priority queues select the cluster with the min cost.
    auto compareClustersBuild = [](std::pair<LightCluster*, float>& a, std::pair<LightCluster*, float>& b){
        return a.second > b.second;
    };
    // as above but opposite order (building choosing roughest clusters to refine).
    auto compareClustersRun = [](std::pair<LightCluster*, float>& a, std::pair<LightCluster*, float>& b){
        return a.second < b.second;
    };

    std::vector<LightCluster*> LightSystem::ComputeAndApplyLightCuts(
            EisEngine::Vector3 &pos,
            const float &LODDist,
            Shader* activeShader
    ) const {
        // return an empty list if there are no lights registered.
        if(root == nullptr)
            return {};

        //DEBUG_LOG("Computing lightcut for mesh at " + (std::string) pos)

        std::vector<LightCluster*> results = {};

        // custom max-queue with function assessing max error as largest priority.
        auto q = std::priority_queue<
                std::pair<LightCluster*, float>,
                std::vector<std::pair<LightCluster*, float>>,
                decltype(compareClustersRun)>(compareClustersRun);
        // init q with the root and its estimated accuracy/error value.
        q.push({root.get(), root->estimateError(pos)});

        int counter = 0;
        // until there is no more cluster to be subdivided.
        while(!q.empty()){
            counter += 1;
            auto r = q.top();
            q.pop();

            auto node = r.first;
            auto closest = node->bounding_box.GetClosestPointTo(pos);
            auto clusterDist = Vector3::Distance(closest, pos);
            auto error = r.second;

            // calculate threshold dynamically based on cluster distance and object distance from loaders (see RenderingSystem.cpp).
            auto threshold = thresholdFunc(LODDist, clusterDist);

            // if the cluster's computed error factor is good enough,
            // or we're evaluating the light source itself,
            // store the node and continue to the next branch.
            if (error < threshold || node->isLeaf()){
                /*DEBUG_INFO("Storing node with error val " + std::to_string(error) +
                    " (threshold: " + std::to_string(threshold) + "; " +
                    std::to_string(counter) + ". explored)")*/
                results.push_back(node);
                continue;
            }

            // otherwise calculate children's errors and add them to the queue.
            auto lErr = node->left->estimateError(pos);
            q.push({node->left.get(), lErr});

            auto rErr = node->right->estimateError(pos);
            q.push({node->right.get(), rErr});
        }

        //DEBUG_LOG("Computed light cut containing " + std::to_string(results.size()) + " clusters, having explored " + std::to_string(counter) + " clusters.")
        vector<ShaderLightStruct> entries = LightSystem::ToShaderEntries(results);

        // apply clusters to shader
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightCutSSBO);

        size_t newSize = entries.size() * sizeof(ShaderLightStruct);
        // clear & resize buffer to fit
        glBufferData(
            GL_SHADER_STORAGE_BUFFER,
            (GLsizeiptr)newSize,
            nullptr,
            GL_DYNAMIC_DRAW
        );

        // apply data & bind to base location 0.
        glBufferSubData(
            GL_SHADER_STORAGE_BUFFER,
            0,
            (GLsizeiptr)newSize,
            entries.data()
        );
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, lightCutSSBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        activeShader->setInt("nLights", (int)entries.size());

        return results;
    }

    void LightSystem::InsertEntityToBHTree(const int &entityID, const EisEngine::Vector3 &pos) {
        lightsToUpdate.push_back(entityID);
    }

    std::unique_ptr<LightCluster> LightSystem::BuildBHTree() {
        // get a list of light sources
        std::vector<PointLight*> lights = {};
        engine.componentManager->forEachComponent<PointLight>([&](PointLight& light){
            lights.push_back(&light);
        });

        // divide & conquer for a balanced light tree for O(log(n)) lookups
        return BuildBalancedTree(lights, 0, (int) lights.size());
    }


    std::unique_ptr<LightCluster> LightSystem::BuildBalancedTree(
            std::vector<PointLight *> &lights,
            int start,
            int end) {
        // trivial case; only 1 light
        if(end - start == 1) {
            auto light = lights.at(start);
            return std::make_unique<LightCluster>(
                    light,
                    light->GetIntensity(),
                    Bounds3D(light->position())
            );
        }

        // find centroid position & variance
        auto centroid = Vector3();
        for(int i = start; i < end; i++)
            centroid += lights.at(i)->position();
        centroid = centroid * (1.0f / (end - start));

        auto var = Vector3();
        for(int i = start; i < end; i++) {
            auto delta = lights.at(i)->position() - centroid;
            var += Vector3(delta.x * delta.x, delta.y * delta.y, delta.z * delta.z);
        }

        int splitAxis = (var.x >= var.y && var.x >= var.z) ? 0 :
                        var.y >= var.z ? 1 : 2;

        // sort lights[start:end] by vector[splitAxis]
        auto mid = start + (end - start) / 2;

        auto left = BuildBalancedTree(lights, start, mid);
        auto right = BuildBalancedTree(lights, mid, end);

        auto rep = left->total_intensity > right->total_intensity ? left->representative : right->representative;
        auto iTotal = left->total_intensity + right->total_intensity;

        auto cluster = std::make_unique<LightCluster>(
            rep,
            iTotal,
            Bounds3D::Merge(left->bounding_box, right->bounding_box)
        );
        cluster->AddChild(left, true);
        cluster->AddChild(right, false);

        return cluster;
    }


    void LightSystem::RemoveClusterFromBHTree(EisEngine::systems::LightCluster *cluster) {
        // again, this function is still under construction.
        // Currently prunes the cluster and attaches its sibling to its grandparent (pruning indirectly kills the direct father cluster)
        // if the parent cluster is the root, the sibling is named the root, and if the cluster to be removed is the root cluster, all lights are dereferenced from the scene.
        auto affectedLight = cluster->representative;

        // edge case: cluster is root:
        if(cluster == root.get()){
            // no more lights in scene
            root = nullptr;
            return;
        }

        auto parent = cluster->parent;
        bool siblingIsLeft = parent->left.get() != cluster;

        // edge case: cluster is child of root
        if(parent == root.get()){
            // make sibling root
            auto sibling = parent->DetachChild(siblingIsLeft);
            root = std::move(sibling);
            return;
        }

        // get family tree
        auto grandparent = parent->parent;
        // establish left or right sub-branch by comparing pointer addresses.
        bool parentIsLeft = grandparent->left.get() == parent;

        // unlink parent from grandparent & sibling from parent
        auto orphanParent = grandparent->DetachChild(parentIsLeft);
        auto orphanSibling = parent->DetachChild(siblingIsLeft);

        // attach sibling to grandparent (bounding boxes can only get smaller so works)
        grandparent->AddChild(orphanSibling, parentIsLeft);
        grandparent->UpdateBBox();

        // link sibling to grandparent -> fine because bounding box can only get smaller
        // if granparent.representative isn't cluster.representative can return safely
        if (grandparent->representative != affectedLight)
            return;

        // otherwise rechoose representative & propagate up (ChooseRepresentative() already handles recursion)
        grandparent->ChooseRepresentative();
    }

#pragma endregion

#pragma region helper functions
    vector<ShaderLightStruct> LightSystem::ToShaderEntries(vector<LightCluster*>& clusters) {
        // pre-allocate memory, target size is known.
        vector<ShaderLightStruct> entries = {};
        entries.reserve(clusters.size());


        for (auto& cluster : clusters) {
            entries.push_back({
                cluster->representative->position(),        // light source direction
                0,                                          // memory padding for SSBO readability
                cluster->representative->GetEmission(),     // material data from representative light source
                cluster->total_intensity                    // cluster intensity - rework to query its current sum of intensities instead of the cached value, or update the cache once per frame.
            });
        }

        return entries;
    }
#pragma endregion
}

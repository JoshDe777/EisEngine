#include "engine/utilities/Bounds3D.h"
#include "engine/components/PointLight.h"

namespace EisEngine {
    
    using Bounds3D = EisEngine::utilities::Bounds3D;

    namespace systems {
        using PointLight = EisEngine::components::PointLight;
        /// A struct extended to a class used to approximate lighting aggregating any child light clusters.
        class LightCluster {
        public:
            /// Creates a new Light Cluster.
            /// @param representative: PointLight* - a pointer to the strongest light source in the cluster, to approximate material data aggregation.
            /// @param intensity: float - The combined intensity of all light sources in this cluster.
            /// @param bounds: float[6] - the 3D bounds of the cluster, in order x_min, x_max, y_min, y_max, z_min, z_max.
            explicit LightCluster(
                PointLight* representative,
                const float& intensity,
                std::array<float, 6> bounds
            ) :
                representative(representative),
                total_intensity(intensity),
                bounding_box(bounds) {
            }

            /// Creates a new Light Cluster.
            /// @param representative: PointLight* - a pointer to the strongest light source in the cluster, to approximate material data aggregation.
            /// @param intensity: float - The combined intensity of all light sources in this cluster.
            /// @param bounds: Bounds3D - the 3D bounds of the cluster.
            explicit LightCluster(
                PointLight* representative,
                const float& intensity,
                Bounds3D bounds
            ) :
                representative(representative),
                total_intensity(intensity),
                bounding_box(bounds) {
            }

            /// Determines whether a cluster is a leaf cluster or not (does it have children?)
            [[nodiscard]] bool isLeaf() const { return left == nullptr && right == nullptr; }

            // A tuple of pointers to child clusters. Can be empty.
            // Children must contain a LightCluster with this->representative == child->representative if not empty.

            /// A proprietary (unique) pointer towards the child cluster on the left (left cluster should be the child cluster with the strongest combined intensity).
            std::unique_ptr<LightCluster> left = nullptr;
            /// A proprietary (unique) pointer towards the child cluster on the right (child cluster with the weaker combined intensity).
            std::unique_ptr<LightCluster> right = nullptr;
            /// A pointer to a cluster's parent cluster. Can be null for the root cluster.
            LightCluster* parent;

            /// A pointer to the cluster's representative light source for material data.
            PointLight* representative = nullptr;

            /// A sum of the intensity of all children in the bounding box.
            float total_intensity = 0;

            /// x_min, x_max, y_min, y_max, z_min, z_max for the lights in the box
            Bounds3D bounding_box;

            /// (Deterministic Barnes-Hut) Determines a cluster's error value.
            [[nodiscard]] float estimateError(const Vector3& pos) const;


    #pragma region parenting stuff
            /// Marks the passed light cluster as its child.
            /// @param cluster: unique_ptr[LightCluster]& - a reference to a unique pointer of the child-to-be.
            /// @param toLeft: bool - Determines whether to assign the child to the left or not. 
            void AddChild(std::unique_ptr<LightCluster>& cluster, bool toLeft) {
                cluster->parent = this;

                // this potentially overwrites any child that would already be there?
                if (toLeft)
                    left = std::move(cluster);
                else
                    right = std::move(cluster);
            }

            /// Detaches a selected child from the current node, and returns a unique pointer of the new orphan.
            /// @param _left: bool - select the left child or not
            /// @returns unique_ptr[LightCluster]
            std::unique_ptr<LightCluster> DetachChild(bool _left) {
                if (_left)
                    return std::move(left);
                return std::move(right);
            }

            /// Updates the bounding box when a child has been modified. Recursively calls on parent clusters to update as well.
            void UpdateBBox() {
                // no updates if the cluster is at most one child. Seems like a bit of a flawed assumption to make icl, but the tree is designed to not have single children.
                if (left == nullptr || right == nullptr)
                    return;

                // merge the bounding boxes and overwrite the box.
                bounding_box = Bounds3D::Merge(left->bounding_box, right->bounding_box);

                // recurse up if the cluster has a parent.
                if (parent != nullptr)
                    parent->UpdateBBox();
            }

            /// Updates the representative light source (choosing left.representative).
            void ChooseRepresentative() {
                // exit early if no children or representative is already correct.
                if (isLeaf() || (representative == left->representative))
                    return;

                representative = left->representative;
                // propagate up if changed.
                if (parent != nullptr)
                    parent->ChooseRepresentative();
            }
    #pragma endregion
        };
    }
}
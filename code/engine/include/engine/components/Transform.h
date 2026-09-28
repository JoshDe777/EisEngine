#pragma once

#include <set>
#include <glm/glm.hpp>
#include "engine/ecs/Component.h"
#include "engine/Utilities.h"

using EisEngine::ecs::Component;
using EisEngine::ecs::ComponentManager;
using EisEngine::ecs::Entity;

namespace EisEngine{
    namespace systems{ class SceneGraphUpdater;}

    namespace components {
        class PhysicsBody2D;

        /// Represents an entity's transform data.
        /// It is heavily advised NOT to add more than one transform to an entity,
        /// which automatically creates one on creation.
        class Transform final: public Component {
            friend ComponentManager;
            friend Entity;
            friend PhysicsBody2D;
            friend systems::SceneGraphUpdater;
        public:
            /// Creates a new transform.
            /// @param parentTransform: Transform* - a pointer to a transform designated as a parent. Can be null.
            /// @param position: Vector3 - A starting position in world space. Defaults to the origin (0, 0, 0).
            /// @param rotation: Vector3 - A starting Euler rotation in world space. Defaults to no rotation (0, 0, 0)
            /// @param scale: Vector3 - A starting scale in world space. Defaults to unscaled (1, 1, 1)
            explicit Transform(Game &engine,
                               guid_t owner,
                               Transform *parentTransform = nullptr,
                               Vector3 position = Vector3::zero,
                               Vector3 rotation = Vector3::zero,
                               Vector3 scale = Vector3::one);

            Transform(const Transform&) = delete;
            Transform& operator = (const Transform&) = delete;
            Transform& operator = (Transform&&) = delete;
            Transform(Transform &&other) noexcept;

            /// Function called when the component is marked for deletion as a component
            void Invalidate() override;

            /// Returns the object's position relative to the world origin.
            [[nodiscard]]Vector3 GetGlobalPosition();
            /// Returns the object's rotation relative to the world origin.
            [[nodiscard]]Vector3 GetGlobalRotation() const;
            /// Returns the object's scale relative to the world origin.
            [[nodiscard]] Vector3 GetGlobalScale();
            /// Returns the object's position relative to its parent object.
            [[nodiscard]]Vector3 GetLocalPosition(){ return localPosition;}
            /// Returns the object's rotation relative to its parent object.
            [[nodiscard]]Vector3 GetLocalRotation(){ return localRotation;}
            /// Returns the object's scale relative to its parent object.
            [[nodiscard]] Vector3 GetLocalScale(){ return localScale;}
            /// Determines whether a transform was affected by an alteration this frame.
            [[nodiscard]] bool IsDirty() const { return dirty;}

            /// Returns the transform's model matrix, condensing full transform data in one object.
            [[nodiscard]] glm::mat4 GetModelMatrix();
            /// Returns a transform's local model matrix, condensing local transform data into one object.
            glm::mat4 GetLocalMatrix();

            /// Sets transform position in world space.
            void SetGlobalPosition(const Vector3& pos);
            /// Sets transform rotation in world space.
            void SetGlobalRotation(Vector3& rotation);
            /// Sets transform scale in world space.
            void SetGlobalScale(const Vector3& scale);
            /// Sets transform position relative to its parent object.
            void SetLocalPosition(const Vector3& pos);
            /// Sets transform rotation relative to its parent object.
            void SetLocalRotation(Vector3& rotation);
            /// Sets transform scale relative to its parent object.
            void SetLocalScale(const Vector3& scale);

            /// Gets an entity's child entities in the scene graph.
            /// @return std::set&lt;Transform *>: \n a set of pointers to the child entities' Transform components.
            std::set<Transform*> getChildren() { return children;}
            /// Gets the transform's parent transform.
            /// @return Transform*: a pointer to the parent transform component.
            Transform *parent() { return m_parent;}
            /// Sets a new parent transform.
            /// @param transform - Transform*: a pointer to the parent-to-be transform.
            void SetParent(Transform *transform);

            /// Moves the transform by the given vector.
            void Translate(const Vector3 &direction);
            /// Rotates the transform by the given Euler angles.
            void Rotate(const Vector3 &vector);
            /// Scales the transform with the given vector.
            void Rescale(const Vector3& scalingFactors);

            // All transform::Direction() functions assume objects are created facing negative Z.

            /// Calculates the direction to the right hand side of an object, assuming it started facing negative Z.
            [[nodiscard]] Vector3 Right() const { return Vector3::right.Rotate(localRotation).normalized();}
            /// Calculates the upwards direction of an object, assuming it started facing negative Z.
            [[nodiscard]] Vector3 Up() const { return Vector3::up.Rotate(localRotation).normalized();}
            /// Calculates the forwards direction of an object, assuming it started facing negative Z.
            [[nodiscard]] Vector3 Forward() const { return Vector3::forward.Rotate(localRotation).normalized();}

            /// A debug function used to debug whether child transforms were assigned properly
            /// @param root: bool - Designates whether to .
            void PrintRelativeSceneGraph(bool root = true);
        private:
            /// Adds a child Transform.
            void AddChild(Transform *transform);
            /// Removes a given child from the children list.
            void RemoveChild(Transform *transform);

            /// Represents transform position relative to its parent entity.
            Vector3 localPosition;
            /// Represents transform rotation relative to its parent entity.
            Vector3 localRotation;
            /// Represents transform scale relative to its parent entity.
            Vector3 localScale;
            /// Represents the transform data in global space as a 4x4 matrix.
            glm::mat4 modelMatrix;

            /// Indicates whether the transform's position was changed manually in the current frame.
            bool m_positionChanged = false;
            /// Indicates whether the transform's rotation was changed manually in the current frame.
            bool m_rotationChanged = false;
            /// Indicates whether the transform's position was changed manually in the current frame.
            bool m_scaleChanged = false;
            /// pointer to the transform's parent transform.
            Transform* m_parent = nullptr;
            /// a set of transform pointers assigned as the current transform's children.
            std::set<Transform*> children;
            /// flags whether the object was changed directly in the world.
            bool dirty = true;

            /// Syncs global position to the physics body's.
            void SyncPosition(const Vector3& newPosition);
            /// Syncs global rotation to the physics body's.
            void SyncRotation(Vector3& newRotation);
            /// Syncs global scale to the collider's.
            void SyncScale(const Vector3& oldScale, const Vector3& newScale);

            /// Marks the current transform and all of its children as dirty. Automatically called whenever a position, rotation, or scale is changed or indirectly affected by a parent operation.
            void MarkDirty();
        };
    }
}


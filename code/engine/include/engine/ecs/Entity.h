#pragma once

#include <string>
#include <utility>
#include <vector>
#include "ecs.h"
#include "engine/ecs/ComponentManager.h"
#include "engine/components/Transform.h"

namespace EisEngine::ecs {
    /// Represents an object of a game with data and/or logic attached to it.
    class Entity final {
        friend class EntityManager;
        using Transform = EisEngine::components::Transform;
    public:
        ~Entity() = default;

        /// Gets the entity's unique ID.
        [[nodiscard]] guid_t guid() const { return m_id; }

        /// Determines whether an entity is marked for deletion.
        [[nodiscard]] bool isDeleted() const { return deleted; }
        /// Checks if the given string matches the entity's tag.
        [[nodiscard]] bool CompareTag(const std::string& tag) const { return tag == m_tag;}

        /// Gets the entity's name.
        [[nodiscard]] std::string name() const {return m_name;}
        /// Gets the entity's tag.
        [[nodiscard]] std::string tag() const {return m_tag;}

        /// Changes the entity's tag.
        void setTag(const std::string &tag) { m_tag = tag;}
        /// Changes the entity's name.
        void rename(const std::string &name){ m_name = name;}

        /// Adds a component of the given type to an entity.
        /// @return @a Component& - a reference to the newly instantiated Component.
        template<typename C, typename ...Args>
        C *AddComponent(Args ...args) { return componentManager.addComponent<C>(m_id, args...);}

        /// Gets a component from this entity.
        /// @return Component* - a pointer to the retrieved Component.
        /// returns nullptr if none is found.
        template<typename C>
        C *GetComponent() { return componentManager.getComponent<C>(m_id);}

        /// Removes a specific component from this entity.
        /// @param C - the type of component to be removed.
        /// @param component - a reference to the exact component to be removed.
        template<typename C>
        void RemoveComponent(C* component){
            component->Invalidate();
            componentManager.removeComponent<C>(m_id);
        }

        /// Removes any component from this entity.
        /// @param C - the type of component to be removed.
        template<typename C>
        void RemoveComponent(){
            C* component = GetComponent<C>();
            RemoveComponent<C>(component);
        }

        /// a pointer to the transform assigned to an entity.
        Transform* transform = nullptr;
        /// A void pointer allowing for users to bridge between their systems and an EisEngine entity.
        void* user_data = nullptr;

        bool operator==(Entity& other) const {return other.m_id == m_id;}
    private:
        /// Creates a new entity.
        /// @param name - std::string: the name given to the entity, not necessarily unique.
        /// \n defaults to "Entity".
        /// @param tag - std::string: the tag given to the entity.
        /// \n defaults to "Untagged".
        explicit Entity(guid_t id,
                        std::string name,
                        ComponentManager &componentManager,
                        std::string  tag = "Untagged",
                        void* userData = nullptr);

        /// Cleanses all components assigned to this entity.
        void deleteAllComponents() {
            componentManager.removeComponents(m_id);
        }

        /// the unique ID of this entity.
        guid_t m_id = invalidID;
        /// the name of this entity.
        std::string m_name;
        /// the tag of this entity.
        std::string m_tag;
        /// Determines whether the entity is marked for deletion.
        bool deleted = false;
        /// The component manager, used to create, get and remove components.
        ComponentManager &componentManager;
    };
}

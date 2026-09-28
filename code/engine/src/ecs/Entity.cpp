#include <utility>

#include "engine/ecs/Entity.h"

namespace EisEngine::ecs{
    // constructor
    Entity::Entity(
        guid_t id, 
        string name, 
        ComponentManager &componentManager,
        string tag, 
        void* userData
    ) : m_id(id), 
        m_name(std::move(name)), 
        m_tag(std::move(tag)),
        componentManager(componentManager), 
        user_data(userData) {
        // issue warning if encountering an invalid entity ID
        if(m_id < 0)
            DEBUG_WARN("<Entity::Entity> Invalid Entity ID")

        // automatically create a transform with every entity.
        transform = AddComponent<Transform>();
    }
}

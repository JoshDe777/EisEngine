#pragma once

#include "engine/ecs/Component.h"
#include "engine/Game.h"

namespace EisEngine::ecs{
    // constructor
    Component::Component(EisEngine::Game &engine, EisEngine::ecs::guid_t owner) : owner(owner), engine(engine) { }

    // point-back to owner entity object.
    Entity *Component::entity() const { return engine.entityManager->getEntity(owner);}
}

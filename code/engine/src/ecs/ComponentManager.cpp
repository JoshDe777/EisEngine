#pragma once

#include "engine/ecs/ComponentManager.h"
#include "engine/Game.h"

namespace EisEngine::ecs{
    // constructor
    ComponentManager::ComponentManager(Game &engine) : engine(engine)
    { }
}

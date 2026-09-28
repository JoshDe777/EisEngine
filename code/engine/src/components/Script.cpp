#include "engine/components/Script.h"
#include "engine/Game.h"

namespace EisEngine::components {
    Script::Script(EisEngine::Game &engine, EisEngine::ecs::guid_t owner) : Component(engine, owner) {
        // add the starting code to the loop for a one-time call next frame
        engine.onEntityStart.addListener([&] (Game &engine){ Start();});
        // add the update handle to the loop to be called every frame.
        updateHandle = engine.onUpdate.addListener([&] (Game &engine) { Update();});
    }

    void Script::Invalidate() {
        // remove the class's handle from the update event
        engine.onUpdate.removeListener(updateHandle);
        Component::Invalidate();
    }
}

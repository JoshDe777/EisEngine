#pragma once

#include "engine/ecs/Component.h"

using EisEngine::ecs::Component;

namespace EisEngine::components {
    /// A script component, used to add custom behaviours to entities.
    class Script : public Component {
    public:
        /// Creates a new Script.
        explicit Script(Game &engine, guid_t owner);
        /// This function is called once at the start of the script's lifetime.
        virtual void Start() { }
        /// This function is called once every frame.
        virtual void Update() { }
    protected:
        /// A function called when a component is intentionally deleted.
        void Invalidate() override;
    private:
        /// a handle to the engine's update event for removal on untimely destruction.
        EisEngine::events::Event<Game, Game&>::handle_t updateHandle;
    };
}

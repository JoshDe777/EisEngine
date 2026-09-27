#pragma once

#include "engine/ecs/System.h"
#include "engine/Events.h"

namespace EisEngine {
    using ecs::System;

    namespace systems{
        /// This System handles everything to do with time.
        class Time : public System {
            friend Game;
        public:
            /// Creates a time system.
            /// @param engine - a reference to the engine to access components and entities.
            explicit Time(Game &engine);

            /// Represents the time elapsed between the last two frames.
            static float deltaTime;
        private:
            /// The time elapsed in the past frame.
            float lastFrameTime = 1.0f/60;
            /// the time elapsed in the current frame.
            float _deltaTime = 1.0f/60;

            /// Updates the delta time value.
            void UpdateDeltaTime();
        };
    }
}

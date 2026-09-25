#pragma once

#include "engine/utilities/KeyCode.h"
#include "engine/ecs/System.h"

namespace EisEngine {
    using System = ecs::System;

    class Input: public System {
        friend Game;
    public:
        /// \n Get whether the given keyboard key is pressed in the current frame.
        /// @return true if the given button is being pressed, false otherwise.
        static bool GetKeyDown(KeyCode key);

        /// \n Get whether the left mouse button (LMB) is pressed in the current frame.
        /// @return true if the LMB is being pressed, false otherwise.
        static bool GetLeftMouseButtonDown();

        /// \n Get whether the right mouse button (RMB) is pressed on the current frame.
        /// @return true if RMB is being pressed, false otherwise.
        static bool GetRightMouseButtonDown();
    private:
        /// \n Creates a new input system.
        explicit Input(Game &engine);

        /// \n A pointer to the active window.
        static GLFWwindow *window;
    };
}

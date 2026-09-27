#pragma once

#include <string>
#include <functional>
#include <OpenGL/OpenGlInclude.h>
#include "engine/utilities/Vector2.h"

namespace EisEngine::ctx {
    /// A class handling all the relevant background information of a game.
    class Context {
    public:
        using Callback = std::function<void(Context&)>;

        /// Creates a context instance.
        /// @param width - the width of the game window.
        /// @param height - the height of the game window.
        /// @param title - the title of the game window.
        explicit Context(const std::string &title = "Game");
        ~Context();

        /// Begins the run of a window context.
        /// @param update - a callback function to be executed each frame.
        void run(const Callback& update);

        /// Gives the signal for the active window to close.
        void CloseWindow(){ glfwSetWindowShouldClose(window, true);}

        /// Fetches a pointer to the window.
        /// @return @a GLFWwindow* - a pointer to the game's window.
        [[nodiscard]] GLFWwindow *getWindow() { return window; }

        /// Gets the dimensions of the game window by **editing** the provided width and height variables.
        Vector2 GetWindowSize() {
            int width = 0;
            int height = 0;
            glfwGetWindowSize(window, &width, &height);
            return Vector2(width, height);
        }
    private:
        /// Initializes OpenGL window parameters.
        static void InitializeGLFW();
        /// Creates a new window.
        /// @param title - std::string: window title.
        void createWindow(const std::string &title);

        /// Initializes and loads Glad.
        static void LoadGLAD();
        /// Pointer to the active window.
        GLFWwindow* window = nullptr;
        /// Initializes and loads ImGUI
        void LoadImGUI();
    };
}

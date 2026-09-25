#include "engine/systems/Input.h"
#include "engine/Game.h"

namespace EisEngine {
    GLFWwindow *Input::window = nullptr;

    Input::Input(EisEngine::Game &engine) : System(engine) { window = engine.getWindow();}

    bool Input::GetKeyDown(EisEngine::KeyCode key) {
        if (window == nullptr) return false;
        return glfwGetKey(window, (int) key) == GLFW_PRESS;
    }

    bool Input::GetLeftMouseButtonDown() {
        if (window == nullptr) return false;
        return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    }

    bool Input::GetRightMouseButtonDown() {
        if (window == nullptr) return false;
        return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
    }
}
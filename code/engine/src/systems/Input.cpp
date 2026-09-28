#include "engine/systems/Input.h"
#include "engine/Game.h"

namespace EisEngine {
    // uses static variables to access using Input:: instead of having to get a reference.
    GLFWwindow *Input::window = nullptr;
    Vector2 Input::_mousePos = Vector2();
    Vector2 Input::_mouseDelta = Vector2();
    float Input::_mouseScroll = 0;

    // init stuff. Ignored after first frame.
    static bool firstCall = true;

    void Input::MouseCallback() {
        // get mouse info. Just mouse position and the differential between current and last frame for now.
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        auto currentPos = Vector2((float) x, (float) y);
        auto lastPos = _mousePos;

        if(firstCall){
            lastPos = currentPos;
            firstCall = false;
        }

        _mouseDelta = lastPos - currentPos;
        _mousePos = currentPos;
    }

    void Input::ScrollCallback(GLFWwindow *window, double x, double y) {_mouseScroll = (float) y;}

    Input::Input(EisEngine::Game &engine) : System(engine) {
        window = engine.getWindow();
        glfwSetScrollCallback(window, Input::ScrollCallback);
        // collect mouse input before logic processing.
        engine.onBeforeUpdate.addListener([&] (Game& game){
            MouseCallback();
        });
    }

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
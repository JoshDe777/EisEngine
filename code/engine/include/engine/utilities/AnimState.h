#pragma once

#include <functional>
#include <utility>

namespace EisEngine {
    /// Represents a specific state in an animation state machine, running an animation when active.
    class AnimState {
    public:
        /// Creates a new AnimState object.
        /// @param onRun: function[param: float, return: void] - A function called when the anim state is active.
        /// @param onEntering: function[param: none, return:void] - A function called once when entering activity on this state.
        /// @param onExiting: function[param: none, return: void] - A function called once when exiting activity from this state.
        /// @param exitCondition: function[param: none, return: bool] - A function called once per frame when active, to evaluate whether to exit from state or not.
        explicit AnimState(
                std::function<void(const float&)> onRun,
                std::function<void()> onEntering,
                std::function<void()> onExiting,
                std::function<bool()> exitCondition) :
                onRun(std::move(onRun)), onEntering(std::move(onEntering)),
                onExiting(std::move(onExiting)), exitCondition(std::move(exitCondition)) {}
        /// A function called once per frame when active.
        /// @returns a boolean indicating whether the state's exit conditions were met.
        bool run(const float& elapsedTime);
        /// A function called once when entering activity on this state.
        void onEnter() { onEntering();}
    private:
        /// A function called when the anim state is active.
        std::function<void(const float&)> onRun;
        /// A function called once when entering activity on this state.
        std::function<void()> onEntering;
        /// A function called once when exiting activity from this state.
        std::function<void()> onExiting;
        /// A function called once per frame when active, to evaluate whether to exit from state or not.
        std::function<bool()> exitCondition;
    };
}

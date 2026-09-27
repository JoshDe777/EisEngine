#pragma once

#include <memory>

#include "engine/ecs/Component.h"
#include "engine/Events.h"
#include "engine/utilities/AnimState.h"

namespace EisEngine {
    using namespace ecs;
    namespace components {
        /// This component runs logic relating to the animation of an entity's mesh.
        class Animator : public Component {
        public:
            /// Creates a new Animator component.
            /// @param animStates - vector<shared_ptr<AnimState>>: >A list of animation states for the animator to cycle through.
            explicit Animator(Game& engine, guid_t owner, std::vector<std::shared_ptr<AnimState>>& animStates);

            /// A function called when a component is intentionally deleted.
            void Invalidate() override;
        private:
            /// A method called once per frame to progress the animations.
            void Update();
            /// The set of animation states this Animator contains
            std::vector<std::shared_ptr<AnimState>> states;
            /// The ID of the active animation state.
            /// Defaults to 0.
            unsigned int currentState = 0;
            /// The amount of time passed since entering the current animation state.
            float elapsedTime = 0;
            /// Determines whether the animator switched states in the previous frame.
            bool newState = true;
        };
    }
}

#include "engine/components/Animator.h"
#include "engine/systems/Time.h"

using namespace EisEngine::systems;

namespace EisEngine::components {
    // constructor
    Animator::Animator(
        Game& engine, 
        guid_t owner, 
        std::vector<std::shared_ptr<AnimState>>& animStates
    ) : Component(engine, owner), 
        states(animStates) { }

    // destructor
    void Animator::Invalidate() {
        Component::Invalidate();
    }

    // update
    void Animator::Update(){
        auto activeState = states[currentState].get();

        // reset values if entering new state.
        if(newState){
            activeState->onEnter();
            elapsedTime = 0.0f;
            newState = false;
        }

        // run active state & update elapsed time.
        if(activeState->run(elapsedTime)){
            newState = true;
            currentState++;
        }
        elapsedTime += Time::deltaTime;
    }
}

#include "TaskAvoidCollisions.h"

#include "../Scripts/EnemyController.h"
#include "../GameObjects/EnemyShip.h"

namespace Ocean_Outlaws::AI {
    TaskAvoidCollisions::TaskAvoidCollisions(BehaviourTree *bTree) : BTNode((bTree)) { }
    TaskAvoidCollisions::TaskAvoidCollisions(BehaviourTree *bTree, std::initializer_list<BTNode *> children) :
    BTNode(bTree, children) { }

    NodeState TaskAvoidCollisions::Evaluate() {
        // call an error if the AI ship can't be found.
        if(!ship){
            auto controller = dynamic_cast<EnemyController*>(tree);
            if(!controller)
                DEBUG_RUNTIME_ERROR("Invalid dynamic cast <BehaviourTree to EnemyController>")
            ship = controller->GetShip();
            if(!ship)
                DEBUG_RUNTIME_ERROR("Invalid ship")
        }

        auto shipPos = ship->transform->GetGlobalPosition();

        // dictionary readings
        Vector3 dangerForwardVector = Vector3::zero;
        Vector3 dangerPos = Vector3::zero;
        bool isStatic = true;

        tree->TryGetData("crash forward", dangerForwardVector);
        tree->TryGetData("crash pos", dangerPos);
        tree->TryGetData("crash mobility", isStatic);

        // avoiding static object - slow down while turning away
        if(isStatic){
            ship->ReefSail();
            pointToFace = shipPos - (dangerPos - shipPos).normalized() * safetyDistance;
            ship->TurnToFace(pointToFace);
        }
        // avoiding moving object.
        else {
            pointToFace = dangerPos - dangerForwardVector * safetyDistance;
            ship->TurnToFace(pointToFace);

            if(ship->GetSailDeployment() > 0)
                ship->ReefSail();
            else
                ship->DeploySail();
        }

        auto forward = ship->transform->Up();
        if(Math::RadiansToDegrees(
                Vector3::Angle(forward, pointToFace - shipPos)) < 5 &&
                Vector3::DotProduct(forward, pointToFace - shipPos) > 0) {
            tree->RemoveData("crash forward");
            tree->RemoveData("crash pos");
            tree->RemoveData("crash mobility");
        }

        return RUNNING;
    }
}

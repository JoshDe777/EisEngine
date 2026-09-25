#include "CheckPlayerInFiringRange.h"
#include "BehaviourTree/BehaviourTree.h"
#include "../GameObjects/PlayerShip.h"

namespace Ocean_Outlaws::AI {
    CheckPlayerInFiringRange::CheckPlayerInFiringRange(BehaviourTree *bTree) : BTNode(bTree) { }

    CheckPlayerInFiringRange::CheckPlayerInFiringRange(BehaviourTree *bTree, std::initializer_list<BTNode*> children) :
    BTNode(bTree, children) { }

    NodeState CheckPlayerInFiringRange::Evaluate() {
        // fail if no player object found.
        if(!player){
            auto object = GameObject::Find("Player");
            auto cast = dynamic_cast<PlayerShip*>(object);
            if(!cast)
                return FAILURE;
            player = cast;
        }

        auto playerPos = player->transform->GetGlobalPosition();
        auto currentPos = tree->entity()->transform->GetGlobalPosition();

        auto scan = Vector3::Distance(playerPos,currentPos) < scanDistance;

        // if scan was unsuccessful, remove target variable and return fail.
        if(!scan){
            tree->RemoveData("firing target");
            return FAILURE;
        }

        // update target position and return success.
        tree->SetData("firing target", playerPos);
        return SUCCESS;
    }
}

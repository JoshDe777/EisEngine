#include "CheckPlayerNode.h"

#include "BehaviourTree/BehaviourTree.h"
#include "../GameObjects/PlayerShip.h"
#include "Pathfinding.h"

namespace Ocean_Outlaws::AI {
    CheckPlayerNode::CheckPlayerNode(BehaviourTree *bTree) : BTNode(bTree) { }
    CheckPlayerNode::CheckPlayerNode(BehaviourTree *bTree, std::initializer_list<BTNode *> children) :
    BTNode(bTree, children) { }

    NodeState CheckPlayerNode::Evaluate() {
        // fail if no player found
        if(!player){
            auto cast = dynamic_cast<PlayerShip*>(GameObject::Find("Player"));
            if(!cast)
                DEBUG_RUNTIME_ERROR("No player object found!")
            player = cast;
        }

        auto playerPos = player->transform->GetGlobalPosition();
        auto enemyPos = tree->entity()->transform->GetGlobalPosition();

        if(Vector3::Distance(playerPos, enemyPos) > scanDistance) {
            tree->RemoveData("target sea node");
            return FAILURE;
        }

        SeaNode* targetNode = Pathfinding::FindClosestSeaNode(playerPos);
        tree->SetData("target sea node", targetNode);

        return SUCCESS;
    }
}

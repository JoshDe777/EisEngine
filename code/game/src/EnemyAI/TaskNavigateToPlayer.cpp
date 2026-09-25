#include "TaskNavigateToPlayer.h"

#include "Pathfinding.h"
#include "BehaviourTree/BehaviourTree.h"
#include "../GameObjects/EnemyShip.h"
#include "../Scripts/MapGenerator.h"

namespace Ocean_Outlaws::AI {
    TaskNavigateToPlayer::TaskNavigateToPlayer(BehaviourTree *bTree) : BTNode(bTree) { }

    TaskNavigateToPlayer::TaskNavigateToPlayer(BehaviourTree *bTree, std::initializer_list<BTNode *> children) :
    BTNode(bTree, children) { }

    NodeState TaskNavigateToPlayer::Evaluate() {
        // find ship object if none saved.
        if(!ship){
            auto gameObject = static_cast<GameObject*>(tree->entity()->user_data);
            auto cast = dynamic_cast<EnemyShip*>(gameObject);
            if(!cast)
                DEBUG_RUNTIME_ERROR("Could not find enemy object")
            ship = cast;
        }

        SeaNode* data = nullptr;
        // case no navigation data available
        if(!tree->TryGetData("target sea node", data)){
            currentTargetNode = nullptr;
            nextNodeInPath = nullptr;
            activePath = {};
            return FAILURE;
        }

        // if new target point defined
        if(data != currentTargetNode){
            currentTargetNode = data;

            // if no target point cancel navigation.
            if(!currentTargetNode)
                return FAILURE;

            auto start = Pathfinding::FindClosestSeaNode(ship->transform->GetGlobalPosition());
            activePath = Pathfinding::AStarToNode(start, currentTargetNode, keyNodeStep);
            nextNodeInPath = activePath[0];
        }

        // check if reached current waypoint
        if(Vector3::Distance(ship->transform->GetGlobalPosition(), nextNodeInPath->coordinates * 2) < distToReachTile){
            std::string currentCoords;
            if(currentTargetNode)
                currentCoords = nextNodeInPath->coordinates;

            activePath.erase(activePath.begin());
            if(activePath.empty())
                return FAILURE;

            std::string newCoords = activePath[0]->coordinates;

            nextNodeInPath = activePath[0];
        }

        auto tilePos = SeaNodeGraph::SeaNodeToWorldPos(nextNodeInPath);

        ship->TurnToFace(tilePos);

        // calculate turn angle. If > 45° reef sail. if < 15° raise it.
        auto direction = tilePos - ship->transform->GetGlobalPosition();
        auto angle = Math::RadiansToDegrees(Vector3::Angle(ship->transform->Up(), direction));

        if(angle > minSlowDownAngle)
            ship->ReefSail();
        else if (angle < maxSpeedUpAngle)
            ship->DeploySail();

        return RUNNING;
    }
}

#pragma once

#include "BehaviourTree/BTNode.h"

#include "../MapGeneration/SeaNode.h"

namespace Ocean_Outlaws{
    class EnemyShip;
    namespace AI {
        using namespace map;

        /// \n An AI behaviour node that navigates along a path towards the player.
        class TaskNavigateToPlayer : public BTNode {
        public:
            /// \n Creates a TaskNavigateToPlayer node.
            explicit TaskNavigateToPlayer(BehaviourTree* bTree);
            /// \n {Warning - useless} Creates a TaskNavigateToPlayer node with a list of child nodes.
            TaskNavigateToPlayer(BehaviourTree* bTree, std::initializer_list<BTNode*> children);
            /// \n A method called once every frame.
            NodeState Evaluate() override;
        private:
            /// \n A pointer to the node closest to the player (target node)
            SeaNode* currentTargetNode = nullptr;
            /// \n A pointer to the next node in the path to the player.
            SeaNode* nextNodeInPath = nullptr;
            /// \n A list of nodes comprised of the waypoint path to the player.
            std::vector<SeaNode*> activePath = {};

            /// \n a pointer to the enemy AI ship.
            EnemyShip* ship = nullptr;

            /// \n The amount of nodes skipped between two waypoints
            /// (the higher this value, the smoother movement will be, at risk of collisions on too sharp turns).
            const int keyNodeStep = 4;
            /// \n The distance at which it is deemed the enemy has reached a waypoint
            const float distToReachTile = 8;

            /// \n The minimum angle at which the enemy decides to slow down.
            const float minSlowDownAngle = 45;
            /// \n The maximum angle at which the enemy decides to accelerate.
            const float maxSpeedUpAngle = 15;
        };
    }
}

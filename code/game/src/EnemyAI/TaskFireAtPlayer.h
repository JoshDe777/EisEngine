#pragma once

#include "BehaviourTree/BTNode.h"

namespace Ocean_Outlaws{
    class EnemyShip;
    namespace scripts { class EnemyController;}

    namespace AI {
        using namespace scripts;

        /// \n An AI behaviour node tasked with circling around the player and firing cannons.
        class TaskFireAtPlayer : public BTNode {
        public:
            /// \n Creates a new TaskFireAtPlayer node.
            explicit TaskFireAtPlayer(BehaviourTree* bTree);
            /// \n {Warning - useless} Creates a new TaskFireAtPlayer node with a list of child nodes.
            TaskFireAtPlayer(BehaviourTree* bTree, std::initializer_list<BTNode*> children);
            /// \n A method called once every frame.
            NodeState Evaluate() override;
        private:
            /// \n Sets up the node for proper work.
            void Setup();
            /// \n A pointer to the enemy AI instance.
            EnemyController* controller = nullptr;

            /// \n The maximum angle at which a turn is deemed as not necessary.
            float maxNoTurnTolerance = 10;
            /// \n the minimum angle at which the AI will decide to slow down.
            float minSlowDownAngle = 70;
            /// \n the maximum angle at which a ship can be to the player to decide to fire cannons.
            float maxShootingAngle = 20;

            /// \n Turns the ship to point sideways from the player.
            static float Turn(EnemyShip* ship, const float& angleFromStarboard,
                              const float& angleFromPort, const bool& playerInFrontOfEnemy);
            /// \n Manages enemy sail deployment.
            void ManageSails(EnemyShip* ship, const float& turnAngle) const;
            /// \n Determines whether to fire cannons.
            void TryFiring(EnemyShip* ship, const float& turnAngle) const;
        };
    }
}

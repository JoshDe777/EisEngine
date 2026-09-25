#pragma once

#include "BehaviourTree/BTNode.h"

namespace Ocean_Outlaws{
    class PlayerShip;
    namespace scripts { class EnemyController;}

    namespace AI {
        /// \n An AI behaviour node that checks whether the AI is close enough to fire upon the player.
        class CheckPlayerInFiringRange : public BTNode {
        public:
            /// \n Constructs a new CheckPlayerInFiringRange node.
            explicit CheckPlayerInFiringRange(BehaviourTree* bTree);
            /// \n {Warning - useless} Constructs a new CheckPlayerInFiringRange node with a list of child nodes.
            CheckPlayerInFiringRange(BehaviourTree* bTree, std::initializer_list<BTNode*> children);
            /// \n A function called once every frame.
            NodeState Evaluate() override;
        private:
            // \n A pointer to the player.
            PlayerShip* player = nullptr;
            /// \n The max distance at which a player is considered in range.
            float scanDistance = 25;
        };
    }
}

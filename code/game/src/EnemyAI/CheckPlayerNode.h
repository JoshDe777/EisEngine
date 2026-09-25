#pragma once

#include "BehaviourTree/BTNode.h"

namespace Ocean_Outlaws {
    class PlayerShip;
    namespace AI {
        /// \n An AI behaviour node that checks for the player's position and determines the closest navigable tile.
        class CheckPlayerNode : public BTNode {
        public:
            /// \n Creates a new CheckPlayerNode node.
            explicit CheckPlayerNode(BehaviourTree* bTree);
            /// \n {Warning - useless} Creates a new CheckPlayerNode node with a list of child nodes.
            CheckPlayerNode(BehaviourTree* bTree, std::initializer_list<BTNode*> children);
            /// \n A function called once every frame.
            NodeState Evaluate() override;
        private:
            /// \n The scanning range for enemy ships.
            float scanDistance = 200;
            /// \n A pointer to the player.
            PlayerShip* player = nullptr;
        };
    }
}

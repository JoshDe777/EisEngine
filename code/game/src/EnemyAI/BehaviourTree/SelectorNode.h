#pragma once

#include "BTNode.h"

namespace Ocean_Outlaws::AI {
    /// \n Selector node = Logical OR node (Succeeds if at least one child is succeeding).
    /// \n Evaluates every child node once every frame until a child returns a successful operation.
    class SelectorNode : public BTNode {
    public:
        /// \n {Warning - useless} Constructs a new selector node without children.
        explicit SelectorNode(BehaviourTree* bTree);
        /// \n Constructs a new selector node with a list of child nodes.
        SelectorNode(BehaviourTree* bTree, std::initializer_list<BTNode*> children);
        /// \n A function called once per frame.
        NodeState Evaluate() override;
    };
}

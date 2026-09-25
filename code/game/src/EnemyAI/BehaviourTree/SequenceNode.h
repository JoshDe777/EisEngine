#pragma once

#include "BTNode.h"

namespace Ocean_Outlaws::AI {
    /// \n Sequence node = Logical AND gate. Succeeds only if all children succeed.
    /// \n Evaluates each child node every frame and immediately fails once any child also fails.
    class SequenceNode : public BTNode {
    public:
        /// \n {Warning - useless} Constructs a sequence node without any child nodes.
        explicit SequenceNode(BehaviourTree* bTree);
        /// \n Constructs a sequence node with a list of child nodes.
        SequenceNode(BehaviourTree* bTree, std::initializer_list<BTNode*> children);
        /// \n A function called once every frame.
        NodeState Evaluate() override;
    };
}

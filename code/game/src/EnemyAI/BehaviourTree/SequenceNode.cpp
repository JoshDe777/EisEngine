#include "SequenceNode.h"

namespace Ocean_Outlaws::AI {
    SequenceNode::SequenceNode(BehaviourTree *bTree) : BTNode(bTree) { }

    SequenceNode::SequenceNode(BehaviourTree *bTree, std::initializer_list<BTNode*> children) :
    BTNode(bTree, children) { }

    NodeState SequenceNode::Evaluate() {
        auto anyChildIsRunning = false;
        for (auto node : children)
        {
            switch(node->Evaluate())
            {
                case FAILURE:
                    state = FAILURE;
                    return state;
                case SUCCESS:
                    continue;
                case RUNNING:
                    anyChildIsRunning = true;
                    continue;
                default:
                    state = SUCCESS;
                    return state;
            }
        }

        state = anyChildIsRunning ? RUNNING : SUCCESS;
        return state;
    }
}

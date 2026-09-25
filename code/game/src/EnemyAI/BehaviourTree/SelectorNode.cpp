#include "SelectorNode.h"

namespace Ocean_Outlaws::AI {
    SelectorNode::SelectorNode(BehaviourTree *bTree) : BTNode(bTree) { }

    SelectorNode::SelectorNode(BehaviourTree *bTree, std::initializer_list<BTNode*> children) :
    BTNode(bTree, children) { }

    NodeState SelectorNode::Evaluate() {
        if(children.empty())
            return FAILURE;
        for(auto* node : children){
            switch(node->Evaluate()){
                case FAILURE:
                    continue;
                case SUCCESS:
                    state = SUCCESS;
                    return state;
                case RUNNING:
                    state = RUNNING;
                    return state;
                default:
                    continue;
            }
        }

        state = FAILURE;
        return state;
    }
}

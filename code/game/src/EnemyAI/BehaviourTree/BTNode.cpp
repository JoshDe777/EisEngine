#include "BTNode.h"

namespace Ocean_Outlaws::AI {
    BTNode::BTNode(BehaviourTree* bTree) { tree = bTree;}

    BTNode::BTNode(BehaviourTree *bTree, std::initializer_list<BTNode*> children) {
        tree = bTree;
        for(auto& child : children)
            AttachChild(child);
    }

    void BTNode::AttachChild(BTNode* child) {
        child->parent = this;
        children.push_back(child);
    }

    void BTNode::Deconstruct() {
        if(!children.empty()){
            auto childrenCopy = children;
            for(auto child : childrenCopy)
                child->Deconstruct();
        }
        children.clear();
    }
}
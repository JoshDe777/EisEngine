#pragma once

#include <vector>
#include "NodeState.h"

namespace Ocean_Outlaws::AI {
    class BehaviourTree;
    /// \n A node in a behaviour tree. Operations are performed by calling Evaluate().
    class BTNode {
    public:
        /// \n Constructs a new BTNOde without any children.
        explicit BTNode(BehaviourTree* bTree);
        /// \n Constructs a new BTNode with a list of child nodes.
        BTNode(BehaviourTree* tree, std::initializer_list<BTNode*> children);
        /// \n A function called once every frame.
        /// @return NodeState: A state check mirroring the success or failure of a node's operation.
        virtual NodeState Evaluate() { return FAILURE;}

        /// \n A node's parent node.
        BTNode* parent = nullptr;
        /// \n Deconstructs the node, removing children recursively.
        void Deconstruct();
    protected:
        /// \n The current/last saved state of the node.
        NodeState state = FAILURE;
        /// \n A pointer to the behaviour tree the node is assigned to.
        BehaviourTree* tree;
        /// \n A list of child nodes.
        std::vector<BTNode*> children;
    private:
        /// \n Attaches a child node to another node.
        void AttachChild(BTNode* child);
    };
}

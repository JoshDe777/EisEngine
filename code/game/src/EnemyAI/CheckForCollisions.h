#pragma once

#include <EisEngine.h>
#include "BehaviourTree/BTNode.h"

namespace Ocean_Outlaws::AI {
    /// \n An AI behaviour node that checks whether the enemy is on a crash course.
    class CheckForCollisions : public BTNode {
    public:
        /// \n Creates a new CheckForCollisions node.
        explicit CheckForCollisions(BehaviourTree *bTree);
        /// \n {Warning - useless} Creates a new CheckForCollisions node with a list of child nodes.
        CheckForCollisions(BehaviourTree *bTree, std::initializer_list<BTNode *> children);
        /// \n A function called once every frame.
        NodeState Evaluate() override;
    private:
        /// \n The distance at which a collider will be deemed as standing in the way.
        float scanDistance = 10;
        /// \n The width of the scan, to allow for consideration of the enemy's entire body.
        float scanWidth = Math::Sqrt(5);
    };
}

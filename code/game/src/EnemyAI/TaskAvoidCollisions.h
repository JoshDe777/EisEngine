#pragma once

#include <EisEngine.h>

#include "BehaviourTree/BTNode.h"
#include "../Enums.h"

namespace Ocean_Outlaws {
    class EnemyShip;
    namespace scripts { class EnemyController;}

    namespace AI {
        using namespace scripts;
        /// \n An AI behaviour node tasked with avoiding collisions if determined to be on crash course.
        class TaskAvoidCollisions : public BTNode {
        public:
            /// \n Creates a new TaskAvoidCollisions node.
            explicit TaskAvoidCollisions(BehaviourTree* bTree);
            /// \n {Warning - useless} Creates a new TaskAvoidCollisions node with a list of child nodes.
            TaskAvoidCollisions(BehaviourTree* bTree, std::initializer_list<BTNode*> children);
            /// \n A method called once every frame.
            NodeState Evaluate() override;
        private:
            /// \n A pointer to the AI's ship instance.
            EnemyShip* ship = nullptr;
            /// \n A pointer to the physics body in crash path.
            PhysicsBody2D* dangerBody = nullptr;
            /// \n The minimum distance from a body to be deemed out of imminent danger.
            float safetyDistance = 5;
            /// \n The collision 'exit' point targeted for exit from crash courses.
            Vector3 pointToFace = Vector3::zero;
        };

    }
}

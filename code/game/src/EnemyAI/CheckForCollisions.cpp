#include "CheckForCollisions.h"

#include "BehaviourTree/BehaviourTree.h"

namespace Ocean_Outlaws::AI {
    CheckForCollisions::CheckForCollisions(BehaviourTree *bTree) : BTNode(bTree) { }
    CheckForCollisions::CheckForCollisions(BehaviourTree *bTree, std::initializer_list<BTNode *> children) :
    BTNode(bTree, children) {}

    NodeState CheckForCollisions::Evaluate() {
        // if already judged to be in critical danger, let it exit danger situation first;
        bool _;
        if(tree->TryGetData("crash pos", _))
            return SUCCESS;

        auto transform = tree->entity()->transform;
        auto pos = transform->GetGlobalPosition();
        auto left = pos - (transform->Right() * scanWidth);
        auto right = pos + (transform->Right() * scanWidth);
        auto frontRight = right + (transform->Up() * scanDistance);
        auto frontLeft = left + (transform->Up() * scanDistance);
        std::vector<PhysicsBody2D*> collidersInRange =
                PhysicsSystem::FindBodiesWithinBounds(left, right, frontRight, frontLeft);

        if(collidersInRange.empty()){
            tree->RemoveData("crash course collider");
            return FAILURE;
        }

        auto closestColliderDist = std::numeric_limits<float>::max();
        PhysicsBody2D* closestCollider = nullptr;
        auto currentID = tree->entity()->guid();
        for(auto collider : collidersInRange){
            if(collider->entity()->guid() == currentID ||
            collider->entity()->CompareTag("Cannonball") ||
            collider->entity()->CompareTag("Barrel"))
                continue;

            auto dist = Vector3::Distance(pos, collider->entity()->transform->GetGlobalPosition());
            if (dist >= closestColliderDist)
                continue;

            closestColliderDist = dist;
            closestCollider = collider;
        }

        // if not on collision course remove all collision data
        if(!closestCollider){
            tree->RemoveData("crash forward");
            tree->RemoveData("crash pos");
            tree->RemoveData("crash mobility");
            return FAILURE;
        }

        tree->SetData("crash forward", closestCollider->entity()->transform->Up());
        tree->SetData("crash pos", closestCollider->entity()->transform->GetGlobalPosition());
        tree->SetData("crash mobility", closestCollider->GetVelocity() == Vector2::zero);

        return SUCCESS;
    }
}

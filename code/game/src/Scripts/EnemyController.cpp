#include "EnemyController.h"

#include "../EnemyAI/BehaviourTree/SelectorNode.h"
#include "../EnemyAI/BehaviourTree/SequenceNode.h"

#include "../EnemyAI/CheckPlayerNode.h"
#include "../EnemyAI/TaskNavigateToPlayer.h"
#include "../EnemyAI/CheckPlayerInFiringRange.h"
#include "../EnemyAI/TaskFireAtPlayer.h"
#include "../EnemyAI/CheckForCollisions.h"
#include "../EnemyAI/TaskAvoidCollisions.h"

namespace Ocean_Outlaws::scripts {
    EnemyController::EnemyController(Game &engine, guid_t owner, EnemyShip *enemyShip) :
                                     BehaviourTree(engine, owner),
                                     ship(enemyShip) {

    }

    void EnemyController::Update() {
        if(deleted)
            return;
        auto rootNode = GetRoot();
        if(!rootNode)
            DEBUG_RUNTIME_ERROR("Failed to initialize root node for enemy.")
        GetRoot()->Evaluate();
    }

    BTNode* EnemyController::BuildTree() {
        auto* rootNode = new SelectorNode(this, {
            new SequenceNode(this,{
                new CheckForCollisions(this),
                new TaskAvoidCollisions(this)
            }),
            new SequenceNode(this, {
                    new CheckPlayerInFiringRange(this),
                    new TaskFireAtPlayer(this)
            }),
            new SequenceNode(this, {
                new CheckPlayerNode(this),
                new TaskNavigateToPlayer(this)
            })
        });
        return rootNode;
    }
}

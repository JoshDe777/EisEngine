#include "TaskFireAtPlayer.h"

#include "BehaviourTree/BehaviourTree.h"
#include "../Scripts/EnemyController.h"

#include "../GameObjects/EnemyShip.h"

namespace Ocean_Outlaws::AI {
    TaskFireAtPlayer::TaskFireAtPlayer(BehaviourTree *bTree) : BTNode(bTree) { Setup();}

    TaskFireAtPlayer::TaskFireAtPlayer(BehaviourTree *bTree, std::initializer_list<BTNode*> children) :
    BTNode(bTree, children) { Setup();}

    void TaskFireAtPlayer::Setup() { controller = tree->entity()->GetComponent<EnemyController>();}

    NodeState TaskFireAtPlayer::Evaluate() {
        // variable assignment
        auto ship = dynamic_cast<EnemyShip*>(static_cast<GameObject*>(tree->entity()->user_data));
        if(!ship)
            DEBUG_RUNTIME_ERROR("Could not find enemy ship.")

        Vector3 target;
        if(!tree->TryGetData("firing target", target))
            return FAILURE;

        // try turning
        // calculate angle between Transform->Up (forwards) and pos to target
        auto vectorToTarget = target - ship->transform->GetGlobalPosition();
        float turnAngle = 0;
        auto angleToPort = Math::RadiansToDegrees(
                Vector3::Angle(-ship->transform->Right(), vectorToTarget)
                );
        auto angleToStarboard = Math::RadiansToDegrees(
                Vector3::Angle(ship->transform->Right(), vectorToTarget)
                );

        auto dotProduct = Vector3::DotProduct(ship->transform->Up(), vectorToTarget);
        auto playerInFrontOfEnemy = dotProduct > 0;

        // only turn if both angles require a turn.
        if(angleToPort > maxNoTurnTolerance && angleToStarboard > maxNoTurnTolerance)
            turnAngle = Turn(ship, angleToStarboard, angleToPort, playerInFrontOfEnemy);

        // if turn angle sharper than x°, take down sails a bit, otherwise deploy until full
        ManageSails(ship, turnAngle);

        // try firing if turning angle is smaller than y°
        TryFiring(ship, turnAngle);

        return RUNNING;
    }

    float TaskFireAtPlayer::Turn(EnemyShip *ship,
                                const float &angleFromStarboard,
                                const float &angleFromPort,
                                const bool& playerInFrontOfEnemy) {
        /*
         * Explanation:
         * If player is in front of the enemy, you want to turn away from the player ->
         * If starboard (right-hand) side closer, turn counter-clockwise, else (port/left-hand) turn clockwise.
         *
         * If player is behind enemy, you want to turn towards the player ->
         * If starboard side closer, turn clockwise, else (port) turn counter-clockwise.
         */

        auto closestSide = angleFromStarboard < angleFromPort ? STARBOARD : PORT;
        auto turnAngle = closestSide == STARBOARD ? angleFromStarboard : angleFromPort;

        if((playerInFrontOfEnemy && closestSide == STARBOARD) || (!playerInFrontOfEnemy && closestSide == PORT))
            ship->RotateCounterClockwise();
        else
            ship->RotateClockwise();

        return turnAngle;
    }

    void TaskFireAtPlayer::ManageSails(Ocean_Outlaws::EnemyShip *ship, const float &turnAngle) const {
        if(turnAngle >= minSlowDownAngle)
            ship->ReefSail();
        else
            ship->DeploySail();
    }

    void TaskFireAtPlayer::TryFiring(Ocean_Outlaws::EnemyShip *ship, const float &turnAngle) const {
        if(turnAngle < maxShootingAngle)
            ship->TryFiring();
    }
}

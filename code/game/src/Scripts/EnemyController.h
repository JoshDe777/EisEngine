#pragma once

#include "EisEngine.h"

#include "../EnemyAI/Pathfinding.h"
#include "../EnemyAI/BehaviourTree/BehaviourTree.h"

namespace Ocean_Outlaws {
    class EnemyShip;

    namespace scripts {
        using namespace AI;

        class EnemyController : public BehaviourTree {
        public:
            explicit EnemyController(Game &engine, guid_t owner, EnemyShip* enemyShip);
            EnemyShip* GetShip() { return ship;}
        protected:
            void Update() override;
            BTNode* BuildTree() override;
        private:
            EnemyShip* ship;
        };
    }
}

using EnemyController = Ocean_Outlaws::scripts::EnemyController;

#include "EnemyShip.h"

#include "../Scripts/EnemyController.h"

namespace Ocean_Outlaws {
    EnemyShip::EnemyShip(EisEngine::Game &engine, const int &cannonCount, const Vector3& position) :
    Ship(engine, cannonCount, "images/sprites/ships/enemy_ship.png", "Enemy") {
        AddComponent<EnemyController>(this);
        transform->SetGlobalPosition(position);
    }
}

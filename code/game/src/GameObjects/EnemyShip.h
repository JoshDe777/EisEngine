#pragma once

#include "Ship.h"

namespace Ocean_Outlaws {
    /// \n A ship controlled by the enemy AI.
    class EnemyShip : public Ship {
    public:
        /// \n Creates a new enemy ship.
        /// @param cannonCount - the amount of cannons at the AI's disposal.
        /// @param position - the starting position of the enemy ship.
        explicit EnemyShip(Game &engine, const int& cannonCount, const Vector3& position);
    };
}

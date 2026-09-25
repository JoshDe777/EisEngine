#pragma once

#include "Cannonball.h"
#include "../Enums.h"

namespace Ocean_Outlaws {
    /// \n A cannon on a ship.
    class Cannon : public GameObject {
        friend class Ship;
    public:
        /// \n Creates a new Cannon object.
        explicit Cannon(Game &engine, const ShipSide& shipSide);
        /// \n Shoots a bullet.
        void Shoot();
        /// \n A function called once every frame.
        void Update() override;

        /// \n Cannon power.
        float power = 5.0f;
        /// \n The damage dealt by cannonballs shot by the cannon.
        float cannonballDamage = 10.0f;
        /// \n The ship side on which the cannon is placed.
        ShipSide side;
    protected:
        /// \n A method called when a GameObject is intentionally deleted.
        void Invalidate() override;
    private:
        /// \n A list of cannonballs shot from this cannon.
        std::vector<Cannonball*> cannonballs;
        /// \n The relative path from the assets folder to the cannon sprite.
        const fs::path spritePath = "images/sprites/ships/cannon.png";
        /// \n Removes a cannonball from the list of cannonballs.
        void RemoveCannonball(Cannonball &cannonBall);
    };
}

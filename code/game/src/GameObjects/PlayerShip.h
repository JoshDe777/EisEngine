#pragma once

#include "Ship.h"
#include "../ui-elements/SailDeploymentBar.h"
#include "../ui-elements/BoundsExitWarning.h"

namespace Ocean_Outlaws {
    /// \n A ship controlled by the player.s
    class PlayerShip : public Ship {
    public:
        /// \n Creates a new player ship.
        explicit PlayerShip(Game& engine);
    protected:
        /// \n This function is called once every frame.
        void Update() override;
        /// \n A function called when a GameObject is intentionally deleted.
        void Invalidate() override;

        /// \n A function called when the ship's HP reaches 0.
        void onShipwreck() override;
        /// \n A function called when sail deployment values have changed.
        void OnSailDeploymentChanged() override;

        /// \n A function called when the ship exits the map's boundaries.
        void onExitBounds() override;
        /// \n A function called when the ship re-enters the map's boundaries.
        void onReturnToBounds() override;
        /// \n A function called every frame while the player is outside the map's boundaries.
        void OnOutOfBounds() override;

        /// \n A function called whenever the player ship enters collisions.
        void OnCollisionEnter(BoxCollider2D& other) override;
    private:
        /// \n The amount of cannons at the player's disposal.
        int playerShipCannonCount = 4;
        /// \n A pointer to the sail deployment indicator.
        SailDeploymentBar* sailDeploymentBar = nullptr;
        /// \n A pointer to the warning sign instructing the player to return to bounds.
        BoundsExitWarning* boundsWarning = nullptr;
        /// \n The countdown until the player is punished for being out of bounds.
        float boundsCountdown = 0;
        /// \n A pointer to the active camera.
        Transform* camera = nullptr;
        /// \n The camera's z position.
        float cameraZ = 0;
    };
}

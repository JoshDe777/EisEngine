#include "PlayerController.h"
#include "../GameObjects/PlayerShip.h"

namespace Ocean_Outlaws::scripts {
    //region constructors
    PlayerController::PlayerController(Game &engine, guid_t owner, PlayerShip* playerShip) :
            Script(engine, owner),
            ship(playerShip) {
        transform = playerShip->transform;
    }

    PlayerController::PlayerController(Ocean_Outlaws::scripts::PlayerController &&other) noexcept :
    Script(std::move(other)),
    ship(other.ship) {
        transform = other.transform;
    }
    //endregion

    void PlayerController::Update() {
        ManageSail();
        RotateShip();
        TryFiring();
    }

    void PlayerController::ManageSail() {
        if(ship->GetSailDeployment() < 1 && Input::GetKeyDown(KeyCode::W))
            ship->DeploySail();
        if(ship->GetSailDeployment() > 0 && Input::GetKeyDown(KeyCode::S))
            ship->ReefSail();
    }

    void PlayerController::RotateShip() {
        if(Input::GetKeyDown(KeyCode::A))
            ship->RotateCounterClockwise();
        if(Input::GetKeyDown(KeyCode::D))
            ship->RotateClockwise();
    }

    void PlayerController::TryFiring() {
        if(Input::GetKeyDown(KeyCode::Space))
            ship->TryFiring();
    }
}

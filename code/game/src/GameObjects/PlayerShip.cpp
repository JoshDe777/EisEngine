#include "PlayerShip.h"
#include "Barrel.h"
#include "../Scripts/PlayerController.h"
#include "../Scripts/GameManager.h"

namespace Ocean_Outlaws {
    PlayerShip::PlayerShip(EisEngine::Game &engine) :
            Ship(engine,4, "images/sprites/ships/player_ship.png", "Player") {
        // shorter cannon shooting interval because player privilege :3
        cannonRearmTime = 1.0f;

        AddComponent<PlayerController>(this);

        camera = engine.camera.transform;
        cameraZ = camera->GetGlobalPosition().z;

        sailDeploymentBar = new SailDeploymentBar(engine);
    }

    void PlayerShip::Invalidate() {
        Destroy(sailDeploymentBar);
        if(boundsWarning)
            Destroy(boundsWarning);
        Ship::Invalidate();
    }

    void PlayerShip::Update() {
        Ship::Update();
        auto playerPos = transform->GetGlobalPosition();
        camera->SetGlobalPosition(Vector3(playerPos.x, playerPos.y, cameraZ));
    }

    void PlayerShip::onShipwreck() { FindObjectOfType<GameManager>()->GameOver();}

    void PlayerShip::OnSailDeploymentChanged() { sailDeploymentBar->SetDeploymentProgress(GetSailDeployment());}

    void PlayerShip::OnOutOfBounds() {
        if(boundsCountdown < 0){
            TakeDamage(maxHP);
            return;
        }
        boundsCountdown -= Time::deltaTime;
    }

    void PlayerShip::onExitBounds() {
        boundsWarning = new BoundsExitWarning(*engine);
        boundsCountdown = 10;
    }

    void PlayerShip::onReturnToBounds() {
        if(boundsWarning)
            Destroy(boundsWarning);
        boundsCountdown = 0;
    }

    void PlayerShip::OnCollisionEnter(BoxCollider2D &other) {
        Ship::OnCollisionEnter(other);
        if(other.entity()->CompareTag("Barrel")){
            auto barrel = dynamic_cast<Barrel*>(static_cast<GameObject*>(other.entity()->user_data));
            if(!barrel)
                return;
            Heal((float) barrel->healValue);
            barrel->SignalCollected();
        }
    }
}

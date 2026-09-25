#include "Cannon.h"

namespace Ocean_Outlaws {
    Cannon::Cannon(Game &engine, const ShipSide& shipSide) : GameObject(engine, "Cannon", "Cannon"),
    side(shipSide) {
        AddComponent<SpriteRenderer>(spritePath);
        AddComponent<SpriteMesh>();
    }

    void Cannon::Invalidate() {
        auto cannonballsCopy = cannonballs;
        for(auto ball : cannonballs)
            Destroy(ball);
    }

    void Cannon::Shoot() {
        auto* ball = new Cannonball(*engine);
        ball->transform->SetGlobalPosition(transform->GetGlobalPosition());
        auto direction = side == STARBOARD ? transform->parent()->Right() : -transform->parent()->Right();
        ball->SetOff(direction, power, cannonballDamage);
        cannonballs.push_back(ball);
    }

    void Cannon::Update() {
        if(cannonballs.empty())
            return;

        std::vector<Cannonball*> deleteList;
        for(auto ball : cannonballs){
            ball->Update();
            if(ball->deleteThis)
                deleteList.push_back(ball);
        }

        for(auto item : deleteList){
            RemoveCannonball(*item);
            Destroy(item);
        }
    }

    void Cannon::RemoveCannonball(Cannonball &cannonball) {
        auto it = std::remove_if(cannonballs.begin(), cannonballs.end(),
                                 [&cannonball](Cannonball *cb) { return *cb == cannonball; });
        cannonballs.erase(it, cannonballs.end());
    }
}

#include "Barrel.h"

namespace Ocean_Outlaws {
    Barrel::Barrel(Game &engine, const Vector3& pos) : GameObject(engine, "Barrel", "Barrel") {
        transform->SetGlobalPosition(pos);

        AddComponent<SpriteRenderer>(spritePath);
        AddComponent<SpriteMesh>();
        AddComponent<BoxCollider2D>();

        auto body = AddComponent<PhysicsBody2D>();
        body->SetSensorStatus(true);
        body->SetGravityUse(false);

        updateHandle = engine.onUpdate.addListener([&] (Game &engine){ Update();});
    }

    void Barrel::Invalidate() {
        engine->onUpdate.removeListener(updateHandle);
        GameObject::Invalidate();
    }

    void Barrel::SignalCollected() { collected = true;}

    void Barrel::Update() {
        if(collected)
            Destroy(this);
    }
}

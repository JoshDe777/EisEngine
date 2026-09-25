#include "Cannonball.h"

namespace Ocean_Outlaws {
    const fs::path Cannonball::spritePath  = "images/sprites/ships/cannonball.png";
    int Cannonball::cannonBallCount = 0;

    Cannonball::Cannonball(Game &engine) :
            GameObject(engine, "Cannonball", "Cannonball"),
            m_id(cannonBallCount++) {
        AddComponent<SpriteRenderer>(spritePath);
        AddComponent<SpriteMesh>();
        collider = AddComponent<BoxCollider2D>();

        body = AddComponent<PhysicsBody2D>();
        body->SetSensorStatus(true);
        body->SetGravityUse(false);
        body->IgnoreAllCollisions();

        collider->OnCollisionEnter.addListener([&] (BoxCollider2D& other){ OnCollisionEnter(other);});
        collider->OnCollisionExit.addListener([&] (BoxCollider2D& other){ OnCollisionExit(other);});

        transform->SetLocalScale(Vector3(0.2f, 0.2f, 0.2f));
    }

    void Cannonball::SetOff(const Vector3 &dir, const float &power, const float &dmg) {
        direction = dir;
        speed *= power;
        damage = dmg;
        flying = true;
        lifetime = maxLifetime;
    }

    bool Cannonball::Expired() {
        if(lifetime <= 0)
            deleteThis = true;
        return deleteThis;}

    void Cannonball::Update() {
        if(inImmunityPeriod && immunityTimer >= 0)
            immunityTimer -= Time::deltaTime;
        else if (immunityTimer < 0){
            inImmunityPeriod = false;
            body->EnableAllCollisions();
        }

        if(flying){
            transform->Translate(direction * speed * Time::deltaTime);
            if(!Expired())
                lifetime -= Time::deltaTime;
        }

        body->SetVelocity(Vector2::zero);
    }

    void Cannonball::OnCollisionEnter(BoxCollider2D &other) { OnHit.invoke(*this);}

    void Cannonball::OnCollisionExit(BoxCollider2D& other){ deleteThis = true;}
}

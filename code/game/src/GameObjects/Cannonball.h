#pragma once

#include "../GameObject.h"

namespace Ocean_Outlaws {
    /// \n A cannonball roaming the world for a ship to hit.
    class Cannonball : public GameObject {
        using event_t = EisEngine::events::Event<Cannonball, Cannonball&>;
        friend class Cannon;
    public:
        /// \n An event called whenever the cannonball collides with an object.
        event_t OnHit;
        /// \n Creates a new cannonball object.
        explicit Cannonball(Game &engine);

        /// \n Determines whether the cannonball has outlived its lifetime.
        [[nodiscard]] bool Expired();
        /// \n Fires the cannonball into action.
        /// @param dir - the direction in which the cannonball will fly in.
        /// @param power - speed multiplier - represents the cannon's firing power.
        /// @param dmg - the damage dealt to ships on collision.
        void SetOff(const Vector3& dir, const float& power, const float& dmg);
        /// \n The amount of damage dealt by a cannonball
        [[nodiscard]] float GetDamage() const { return damage;}

        /// \n Determines whether the cannonball still ignores collisions (immediately after firing)
        bool inImmunityPeriod = true;
        /// \n Determines whether the cannonball can be deleted.
        bool deleteThis = false;

        bool operator==(Cannonball& other) const{ return m_id == other.m_id;}
    protected:
        /// \n A function called once every frame.
        void Update() override;
    private:
        /// \n The direction in which a cannonball flies.
        Vector3 direction = Vector3::zero;
        /// \n The speed in units per second at which the cannonball flies.
        float speed = 1.0f;
        /// \n Determines whether the cannonball is fired.
        bool flying = false;
        /// \n The amount of damage dealt to ships on impact.
        float damage = 0.0f;

        /// \n The remaining lifetime for a cannonball.
        float lifetime = 0.0f;
        /// \n The remaining immunity time for a cannonball.
        float immunityTimer = 0.3f;

        /// \n The unique cannonball id :tm:
        const int m_id;
        /// \n The max lifetime in seconds of a cannonball.
        const float maxLifetime = 3.5f;

        /// \n The relative path from the assets folder to the cannonball sprite.
        static const fs::path spritePath;
        /// \n The amount of cannonballs fired (results in the cannonball ID).
        static int cannonBallCount;

        /// \n A pointer to the cannonball's collider.
        BoxCollider2D* collider;
        /// \n A pointer to the cannonball's physics body.
        PhysicsBody2D* body;

        /// \n A function called when a cannonball enters collision.
        void OnCollisionEnter(BoxCollider2D& other);
        /// \n A function called when a cannonball exits collision.
        void OnCollisionExit(BoxCollider2D& other);
    };
}

#pragma once

#include "Cannon.h"
#include "../ui-elements/HealthBar.h"

namespace Ocean_Outlaws {
    namespace scripts{
        class GameManager;
        class Wind;
    }
    using namespace UI;

    /// \n A ship navigating through the seas in search of glory or justice.
    class Ship : public GameObject {
        friend class Barrel;
    public:
        using event_s = EisEngine::events::Event<Ship, Ship&>;
        using GameManager = Ocean_Outlaws::scripts::GameManager;
        using Wind = Ocean_Outlaws::scripts::Wind;
        using handle_t = EisEngine::event_t::handle_t;

        /// \n An event invoked when the ship's HP reaches 0.
        event_s OnShipwreck;
    protected:
        /// \n The speed at which a ship gains velocity.
        float acceleration = 0.2f;
        /// \n The maximum speed at which a ship can sail.
        float maxSpeed = 3.0f;
        /// \n The speed at which the crew can manage the sails.
        float sailDeploymentSpeed = 0.5f;
        /// \n The speed at which the ship can turn.
        float rotationSpeed = 25.0f;

        /// \n The amount of cannons at the ship's disposal.
        int cannonCount = 0;
        /// \n The amount of time it takes for the crew to restock a cannon to fire.
        float cannonRearmTime = 5.0f;
        /// \n A list of cannons on board this ship.
        std::vector<Cannon*> cannons;
        /// \n The amount of cannonballs left to fire.
        int ammoCount = 0;

        /// \n The maximum amount of health points a ship can have.
        const float maxHP = 100;

        /// \n A pointer to the ship's renderer.
        SpriteRenderer* renderer = nullptr;
        /// \n A pointer to the ship's mesh.
        SpriteMesh* mesh = nullptr;
        /// \n A pointer to the ship's physics body.
        PhysicsBody2D* body = nullptr;
        /// \n A pointer to the ship's collider.
        BoxCollider2D* collider = nullptr;

        /// \n Determines whether the ship is out of bounds.
        bool isOutOfBounds = false;
    private:
        /// \n The percentage of current sail deployment.
        float sailDeploymentPercent = 0;
        /// \n The time left until cannons can be fired again.
        float cannonCountdown = 0;
        /// \n The current health state of the ship
        float healthPoints = 100;

        /// \n The health indicator for the ship.
        HealthBar healthBar;

        /// \n A pointer to the game manager instance.
        GameManager* gm = nullptr;
        /// \n A pointer to the wind instance.
        Wind* wind = nullptr;

        /// \n The update handle for removal upon destruction.
        handle_t updateHandle;
    public:
        /// \n Creates a new ship.
        explicit Ship(Game &engine,
                      const int& numberCannons,
                      const fs::path& spritePath,
                      const std::string& name = "Ship",
                      const std::string& tag = "Ship");

        /// \n Returns the sail deployment rate.
        [[nodiscard]] float GetSailDeployment() const { return sailDeploymentPercent;}

        /// \n Give the command to fire cannons.
        void TryFiring();

        /// \n Turn the ship clockwise (to starboard)
        void RotateClockwise() const;
        /// \n Turn the ship counter-clockwise (to port)
        void RotateCounterClockwise() const;
        /// \n Turn to face a given point/direction.
        void TurnToFace(const Vector3& direction) const;
        /// \n Raise the ship's sails, increasing speed but also wind exposure.
        void DeploySail();
        /// \n Lower the ship's sails, reducing speed and wind exposure.
        void ReefSail();

        /// \n Recover some health, you deserve it.
        void Heal(const float& value);

        /// \n Returns a pointer to the ship's physics body.
        PhysicsBody2D* GetBody() { return body;}
    protected:
        /// \n A function called when a ship's health falls to 0.
        virtual void onShipwreck();
        /// \n A function called when a ship exits boundaries.
        virtual void onExitBounds() { }
        /// \n A function called when a ship re-enters boundaries.
        virtual void onReturnToBounds() { }
        /// \n A function called once ever frame whilst out of bounds.
        virtual void OnOutOfBounds() { }
        /// \n A function called whenever sails are changed.
        virtual void OnSailDeploymentChanged() { }

        /// \n A function called when a GameObject is intentionally deleted.
        void Invalidate() override;
        /// \n A function called once every frame.
        void Update() override;

        /// \n A function called when a ship enters collision.
        virtual void OnCollisionEnter(BoxCollider2D& other);
        /// \n A function called when a ship exits collision.
        virtual void OnCollisionExit(BoxCollider2D& other);

        /// \n Reduce HP by the given value.
        void TakeDamage(const float& value);
    private:
        /// \n A function called once at the beginning of the ship's lifetime.
        void Start();

        /// \n Loads the ship with cannons.
        void GenerateCannons();
        /// \n Fires all cannons.
        void FireCannons();

        /// \n Moves forwards.
        void MoveForwards();
        /// \n Crew loads the cannons. They're doing their best.
        void UpdateFiringCountdown();
        /// \n Determines whether the ship is in bounds.
        bool InBounds();
    };
}

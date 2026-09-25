#include "Ship.h"
#include "../scripts/GameManager.h"
#include "../scripts/Wind.h"

namespace Ocean_Outlaws {
    Ship::Ship(Game &engine, const int& numberCannons, const fs::path& spritePath,
               const std::string& name, const std::string& tag) :
    GameObject(engine, name, tag), healthBar(engine) {
        healthBar.transform->SetParent(transform);
        healthBar.transform->SetLocalPosition(Vector3(0, 1, 0));

        cannonCount = numberCannons;
        updateHandle = engine.onUpdate.addListener([&] (Game &engine){ Update();});

        transform->SetLocalScale(Vector3(1, 2, 1));
        renderer = AddComponent<SpriteRenderer>(spritePath);

        Start();
    }

    void Ship::Invalidate(){
        engine->onUpdate.removeListener(updateHandle);
        auto cannonsCopy = cannons;
        for(auto cannon : cannonsCopy)
            Destroy(cannon);
        GameObject::Invalidate();
    }

    void Ship::Start() {
        healthPoints = maxHP;
        ammoCount = -1;             // ammo checked with == 0 -> "infinite" ammo;

        mesh = AddComponent<SpriteMesh>();

        collider = AddComponent<BoxCollider2D>();
        auto correctedBounds = *collider->GetBounds();
        correctedBounds.top -= 0.1f;
        correctedBounds.right -= 0.1f;
        correctedBounds.bottom += 0.1f;
        correctedBounds.left += 0.1f;
        correctedBounds.ReevaluateSize();
        collider->SetBounds(correctedBounds);

        body = AddComponent<PhysicsBody2D>();
        body->SetGravityUse(false);

        wind = FindObjectOfType<Wind>();
        gm = wind->entity()->GetComponent<GameManager>();

        GenerateCannons();

        OnShipwreck.addListener([&] (Ship &ship){ onShipwreck();});
        collider->OnCollisionEnter.addListener([&] (BoxCollider2D& other){ OnCollisionEnter(other);});
        collider->OnCollisionExit.addListener([&] (BoxCollider2D& other){ OnCollisionExit(other);});
    }

    void Ship::Update(){
        MoveForwards();
        UpdateFiringCountdown();
        if (!InBounds())
            OnOutOfBounds();

        if(cannons.empty())
            return;
        for(auto cannon : cannons)
            cannon->Update();
    }

    void Ship::MoveForwards() {
        Vector2 force = transform->Up() * acceleration;
        body->AddForce(force * sailDeploymentPercent);
        auto speed = std::min(body->GetVelocity().magnitude(), maxSpeed);
        body->SetVelocity(transform->Up().normalized() * speed);
        float windReception = sailDeploymentPercent;
        if (windReception == 0.0f)
            windReception = 0.1f;

        if(!wind){
            wind = FindObjectOfType<Wind>();
            if(!wind){
                DEBUG_ERROR("No Wind object found!")
                return;
            }
        }
        body->AddForce(wind->GetWind() * windReception);
    }

    void Ship::RotateClockwise() const {
        auto rotation = transform->GetLocalRotation();
        rotation.z -= rotationSpeed * Time::deltaTime;
        transform->SetLocalRotation(rotation);

    }
    void Ship::RotateCounterClockwise() const {
        auto rotation = transform->GetLocalRotation();
        rotation.z += rotationSpeed * Time::deltaTime;
        transform->SetLocalRotation(rotation);
    }

    void Ship::TurnToFace(const Vector3 &point) const {
        auto forward = transform->Up();
        auto direction = point - transform->GetGlobalPosition();

        auto dotProduct = Vector3::DotProduct(forward, direction);
        auto pointInFrontOfShip = dotProduct > 0;

        if(pointInFrontOfShip && Math::RadiansToDegrees(Vector3::Angle(forward, direction)) < 2)
            return;

        auto angleToPort = Math::RadiansToDegrees(
                Vector3::Angle(-transform->Right(), direction)
        );
        auto angleToStarboard = Math::RadiansToDegrees(
                Vector3::Angle(transform->Right(), direction)
        );

        if(angleToStarboard < angleToPort)
            RotateClockwise();
        else
            RotateCounterClockwise();
    }

    void Ship::DeploySail() {
        sailDeploymentPercent += sailDeploymentSpeed * Time::deltaTime;
        sailDeploymentPercent = Math::Clamp(sailDeploymentPercent, 0.0f, 1.0f);
        OnSailDeploymentChanged();
    }
    void Ship::ReefSail() {
        sailDeploymentPercent -= sailDeploymentSpeed * Time::deltaTime;
        sailDeploymentPercent = Math::Clamp(sailDeploymentPercent, 0.0f, 1.0f);
        OnSailDeploymentChanged();
    }

    void Ship::GenerateCannons() {
        if (cannonCount <= 0) {
            DEBUG_WARN("Invalid number of cannons! (" + std::to_string(cannonCount) + ")")
            return;
        }

        if (cannonCount % 2 != 0)
            cannonCount++;

        auto* spriteMesh = GetComponent<SpriteMesh>();

        float xOffsetFromCentreOfSprite = spriteMesh->primitive.bounds.size.x / 3;

        float shipLength = (5 * spriteMesh->primitive.bounds.size.y) / 8;
        float distanceBetweenCannons = 2 * shipLength / (((float) cannonCount / 2) + 1);

        for (int i = 0; i < cannonCount; i++) {
            // distributes cannons to each side. Cannons assigned to even indexes (0, 2...) will point to
            // starboard, whilst the rest will point to port.
            auto cannonDirection = i % 2 == 0 ? ShipSide::STARBOARD : ShipSide::PORT;
            auto cannon = new Cannon(*engine, cannonDirection);
            cannon->transform->SetParent(transform);

            float yPos = (float) shipLength - ((Math::Floor((float) i / 2) + 1) * distanceBetweenCannons);
            float xPos;

            float directionFactor = 1;

            if (cannonDirection == PORT)             // reverse direction for ships pointing to port.
                directionFactor = -1;

            // adjust cannon rotation and x position depending on its ship side.
            cannon->transform->SetLocalRotation(Vector3(0, 0, -90) * directionFactor);
            cannon->transform->SetLocalScale(Vector3(0.3f, 0.3f, 1));

            xPos = directionFactor * xOffsetFromCentreOfSprite;
            cannon->transform->SetLocalPosition(Vector3(xPos, yPos, 0.01f));
            cannons.push_back(cannon);
        }
    }

    void Ship::TryFiring() {
        if(ammoCount == 0 || cannonCountdown > 0)
            return;
        FireCannons();
        cannonCountdown = cannonRearmTime;
    }

    void Ship::FireCannons() {
        if (!cannons.empty()) {
            for (auto cannon : cannons) {
                if (ammoCount != 0){
                    cannon->Shoot();
                    ammoCount--;
                }
                else {
                    ammoCount = 0;
                    DEBUG_LOG("No more ammo!")
                    break;
                }
            }
        }
        else
            DEBUG_LOG("No cannons assigned to ship " + (std::string) *this + "!")
    }

    void Ship::UpdateFiringCountdown() {
        if (cannonCountdown <= 0)
            cannonCountdown = 0;
        else
            cannonCountdown -= Time::deltaTime;
    }

    void Ship::Heal(const float &value) {
        healthPoints += value;
        if(healthPoints > maxHP)
            healthPoints = maxHP;
        healthBar.SetValue(healthPoints / maxHP);
    }

    void Ship::TakeDamage(const float &value) {
        healthPoints -= value;
        healthBar.SetValue(healthPoints / maxHP);
        if(healthPoints <= 0)
            OnShipwreck.invoke(*this);
    }

    void Ship::OnCollisionEnter(BoxCollider2D &other) {
        auto tag = other.entity()->tag();
        if(tag == "Ship")
            TakeDamage(0.5f * maxHP);
        else if(tag == "Land")
            TakeDamage(maxHP);
        else if (tag == "Cannonball"){
            auto cannonball = dynamic_cast<Cannonball*>(static_cast<GameObject*>(other.entity()->user_data));
            // failsafe -> returns nullptr to avoid undefined bhvr.
            if(!cannonball) return;
            TakeDamage(cannonball->GetDamage());
        }
    }

    void Ship::OnCollisionExit(BoxCollider2D &other) { }

    void Ship::onShipwreck() { Destroy(this);}

    bool Ship::InBounds() {
        auto pos = transform->GetGlobalPosition();
        auto mapSize = gm->GetMapWidth();
        auto wasOut = isOutOfBounds;
        isOutOfBounds = (pos.x < 0) ||
                (pos.x > (float) 2 * (float) mapSize) ||
                (pos.y < 0) ||
                (pos.y > (float) 2 * (float) mapSize);
        if(wasOut && !isOutOfBounds)
            onReturnToBounds();
        else if (!wasOut && isOutOfBounds)
            onExitBounds();
        return !isOutOfBounds;
    }
}
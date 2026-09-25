#include "MapTile.h"

namespace Ocean_Outlaws {
    const fs::path MapTile::landTileSpritePath = "images/sprites/tiles/Land.png";

    MapTile::MapTile(EisEngine::Game &engine, const fs::path &spritePath, Transform* parent, Vector3& position) :
    GameObject(engine) {
        position.z = -1;
        transform->SetParent(parent);
        transform->SetGlobalPosition(position);
        transform->SetGlobalScale(Vector3(2, 2, 1));

        bool isLand = spritePath == landTileSpritePath;

        renderer = AddComponent<SpriteRenderer>(spritePath);
        mesh = AddComponent<SpriteMesh>();

        // add colliders for land tiles.
        if(isLand){
            collider = AddComponent<BoxCollider2D>();
            collider->SetBounds(*collider->GetBounds());
            body = AddComponent<PhysicsBody2D>(b2_staticBody);
        }
    }

    void MapTile::UpdateBodyPosition() {
        if(!body)
            return;
        b2Vec2 pos = transform->GetGlobalPosition();
        auto angle = transform->GetGlobalRotation().z;
        body->body->SetTransform(pos, angle);
    }
}

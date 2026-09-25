#pragma once

#include "../GameObject.h"
#include "../MapGeneration/SeaNode.h"

namespace Ocean_Outlaws {
    using map::SeaNode;
    /// \n A tile on the map.
    class MapTile : public GameObject {
    public:
        /// \n Creates a new map tile.
        /// @param spritePath - the relative path to the sprite to be displayed on the tile.
        /// @param position - the tile's position in world space.
        MapTile(Game &engine, const fs::path& spritePath, Transform* parent, Vector3& position);

        /// \n The sprite path for pure land tiles.
        static const fs::path landTileSpritePath;
        /// \n Updates the physics body's position.
        void UpdateBodyPosition();
    private:
        /// \n A pointer to the tile's renderer.
        SpriteRenderer* renderer = nullptr;
        /// \n A pointer to the tile's mesh.
        SpriteMesh* mesh = nullptr;
        /// \n A pointer to the tile's collider.
        BoxCollider2D* collider = nullptr;
        /// \n A pointer to the tile's physics body.
        PhysicsBody2D* body = nullptr;
    };
}

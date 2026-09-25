#pragma once

#include "../GameObject.h"

namespace Ocean_Outlaws {
    /// \n the end screen, displayed when the player dies.
    class EndScreen : public GameObject {
    public:
        /// \n Creates a new end screen instance.
        explicit EndScreen(Game& engine) : GameObject(engine) {
            transform->SetLocalScale(Vector3(spriteSize * 1.374f, spriteSize, 1));
            AddComponent<SpriteRenderer>(spritePath);
            AddComponent<SpriteMesh>();
        }
    private:
        /// \n The sprite size.
        const float spriteSize = 45;
        /// \n The relative asset path to the end screen sprite.
        const fs::path spritePath = "images/dead_screen.png";
    };
}

#pragma once

#include "../GameObject.h"

namespace Ocean_Outlaws {
    /// \n The main menu, displayed before the start of every game.
    class MainMenu : public GameObject {
    public:
        /// \n Creates a new main menu instance.
        explicit MainMenu(Game& engine) : GameObject(engine) {
            transform->SetLocalScale(Vector3(spriteSize * 1.374f, spriteSize, 1));
            AddComponent<SpriteRenderer>(spritePath);
            AddComponent<SpriteMesh>();
        }
    private:
        /// \n The menu sprite' size (height in this case).
        const float spriteSize = 45;
        /// \n The relative path to the main menu sprite.
        const fs::path spritePath = "images/main_menu.png";
    };
}

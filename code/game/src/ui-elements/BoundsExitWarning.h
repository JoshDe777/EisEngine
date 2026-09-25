#pragma once

#include "../GameObject.h"

namespace Ocean_Outlaws::UI {
    class BoundsExitWarning : public GameObject {
    public:
        explicit BoundsExitWarning(Game& engine) : GameObject(engine){
            auto relativeWidth = getRelativeWidth(width, engine.camera.GetWidth());
            auto relativeHeight = getRelativeHeight(height, engine.camera.GetHeight());

            transform->SetLocalScale(Vector3(relativeWidth, relativeHeight, 1));

            auto top = (float) engine.camera.GetHeight() / 2 - relativeHeight / 2;
            transform->SetLocalPosition(Vector3(0, top, 0));

            AddComponent<SpriteRenderer>(spritePath, Color::white, "UI");
            AddComponent<SpriteMesh>();
        }
    private:
        const fs::path spritePath = "images/ui/bounds_warning.png";
        const float width = 500;
        const float height = 200;
    };
}

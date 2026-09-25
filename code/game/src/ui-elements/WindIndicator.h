#pragma once

#include "../GameObject.h"

namespace Ocean_Outlaws::UI {
    class WindIndicator : public GameObject {
    public:
        explicit WindIndicator(Game& engine);
        void UpdateRotation(const float& newRotation);
    protected:
        void Invalidate() override;
    private:
        const fs::path spritePath = "images/ui/wind_indicator.png";
        const fs::path compassSpritePath = "images/ui/compass_rose.png";
        const float spriteSize = 300;
        const float xOffset = 25;
        const float yOffset = 200;

        Entity* compassEntity = nullptr;
    };
}

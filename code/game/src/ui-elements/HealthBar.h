#pragma once

#include "../GameObject.h"

namespace Ocean_Outlaws::UI {
    class HealthBar : public GameObject {
    public:
        explicit HealthBar(Game& engine);
        void SetValue(const float& percent);
    private:
        Entity* sliderEntity;
        const fs::path backgroundSpritePath = "images/ui/health_background.png";
        float defaultXScale = 0;
    };
}

#pragma once

#include "../GameObject.h"

namespace Ocean_Outlaws::UI {
    class SailDeploymentBar : public GameObject {
    public:
        explicit SailDeploymentBar(Game& engine);
        void SetDeploymentProgress(const float& val);
    protected:
        void Invalidate() override;
    private:
        Entity* sliderHandle = nullptr;
        Entity* text = nullptr;
        const fs::path sliderBackgroundPath = "images/ui/sail_background.png";
        const fs::path sliderHandlePath = "images/ui/sail_handle.png";
        const fs::path textPath = "images/ui/sail_text.png";

        float sliderHeight = 1000;
        float sliderWidth = 20;

        float handleHeight = 100;
        float handleWidth = 150;

        float textWidth = 350;
        float textHeight = 100;

        float xOffset = 200;

        void PlaceOnScreen(Camera* camera);
    };
}

#include "HealthBar.h"
#include "glm/gtx/euler_angles.hpp"

namespace Ocean_Outlaws::UI {
    HealthBar::HealthBar(EisEngine::Game &engine) : GameObject(engine) {
        AddComponent<SpriteRenderer>(backgroundSpritePath);
        AddComponent<SpriteMesh>();
        transform->SetLocalScale(Vector3(1.04f, 0.4f, 1));

        sliderEntity = &engine.entityManager.createEntity();
        sliderEntity->transform->SetParent(transform);

        auto sliderShader = &sliderEntity->AddComponent<Mesh2DRenderer>();
        sliderShader->SetColor(Color::green);
        sliderEntity->AddComponent<Mesh2D>(PrimitiveMesh2D::Square);
        defaultXScale = 1;
        sliderEntity->transform->SetLocalScale(Vector3(defaultXScale, 0.3f, 1));
    }

    void HealthBar::SetValue(const float &percent) {
        sliderEntity->transform->SetLocalScale(Vector3(percent * defaultXScale, 0.3f, 1));
        auto newPos = transform->Right() * (percent / 2) - transform->Right() * (defaultXScale/2);
        sliderEntity->transform->SetLocalPosition(newPos);
    }
}

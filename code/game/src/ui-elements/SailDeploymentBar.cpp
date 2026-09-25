#include "SailDeploymentBar.h"
#include "../Enums.h"

namespace Ocean_Outlaws::UI {
    SailDeploymentBar::SailDeploymentBar(EisEngine::Game &engine) : GameObject(engine) {
        sliderHandle = &engine.entityManager.createEntity();
        text = &engine.entityManager.createEntity();
        PlaceOnScreen(&engine.camera);

        AddComponent<SpriteRenderer>(sliderBackgroundPath, Color::white, "UI");
        AddComponent<SpriteMesh>();

        sliderHandle->AddComponent<SpriteRenderer>(sliderHandlePath, Color::white, "UI");
        sliderHandle->AddComponent<SpriteMesh>();

        text->AddComponent<SpriteRenderer>(textPath, Color::white, "UI");
        text->AddComponent<SpriteMesh>();
    }

    void SailDeploymentBar::PlaceOnScreen(Camera *camera) {
        auto width = camera->GetWidth();
        auto height = camera->GetHeight();

        sliderWidth = getRelativeWidth(sliderWidth, width);
        sliderHeight = getRelativeHeight(sliderHeight, height);
        xOffset = getRelativeWidth(xOffset, width);

        auto xCoord = (float) width / 2 - (sliderWidth / 2) - xOffset;
        auto pos = Vector3(xCoord, 0, 0.1f);

        transform->SetGlobalPosition(pos);
        transform->SetLocalScale(Vector3(sliderWidth, sliderHeight, 1));

        pos.y = - sliderHeight / 2;
        pos.z += 0.1f;

        handleWidth = getRelativeWidth(handleWidth, width);
        handleHeight = getRelativeHeight(handleHeight, height);

        sliderHandle->transform->SetGlobalPosition(pos);
        sliderHandle->transform->SetLocalScale(Vector3(handleWidth, handleHeight, 1));

        pos.y = sliderHeight / 2 + getRelativeHeight(150.0f, height);

        textWidth = getRelativeWidth(textWidth, width);
        textHeight = getRelativeHeight(textHeight, height);

        text->transform->SetGlobalPosition(pos);
        text->transform->SetLocalScale(Vector3(textWidth, textHeight, 1));
    }

    void SailDeploymentBar::Invalidate() {
        engine->entityManager.deleteEntity(*sliderHandle);
        engine->entityManager.deleteEntity(*text);
        GameObject::Invalidate();
    }

    void SailDeploymentBar::SetDeploymentProgress(const float &val) {
        auto pos = sliderHandle->transform->GetLocalPosition();
        pos.y = -sliderHeight / 2 + val * sliderHeight;
        sliderHandle->transform->SetLocalPosition(pos);
    }
}
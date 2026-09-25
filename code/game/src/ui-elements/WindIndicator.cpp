#include "WindIndicator.h"
#include "../Enums.h"

namespace Ocean_Outlaws::UI {
    WindIndicator::WindIndicator(EisEngine::Game &engine) : GameObject(engine) {
        auto width = (float) engine.camera.GetWidth();
        auto height = (float) engine.camera.GetHeight();

        auto relativeWidth = getRelativeWidth(spriteSize, width);
        auto relativeHeight = getRelativeHeight(spriteSize, height);
        auto relativeSpriteSize = std::min(relativeWidth, relativeHeight);

        auto relativeXOffset = getRelativeWidth(xOffset, width);
        auto relativeYOffset = getRelativeWidth(yOffset, height);

        auto xCoord = - ((float) width / 2) + relativeSpriteSize /2 + relativeXOffset;
        auto yCoord = - ((float) height / 2) + relativeSpriteSize /2 + relativeYOffset;
        auto pos = Vector3(xCoord, yCoord, 0.1f);

        compassEntity = &engine.entityManager.createEntity();
        compassEntity->transform->SetLocalPosition(pos);
        compassEntity->transform->SetLocalScale(Vector3(relativeSpriteSize, relativeSpriteSize, 1));

        compassEntity->AddComponent<SpriteRenderer>(compassSpritePath, Color::white, "UI");
        compassEntity->AddComponent<SpriteMesh>();

        pos.z += 0.1;

        transform->SetLocalPosition(pos);
        transform->SetLocalScale(Vector3(relativeSpriteSize, relativeSpriteSize, 1));

        AddComponent<SpriteRenderer>(spritePath, Color::white, "UI");
        AddComponent<SpriteMesh>();
    }

    void WindIndicator::Invalidate() {
        engine->entityManager.deleteEntity(*compassEntity);
        GameObject::Invalidate();
    }

    void WindIndicator::UpdateRotation(const float &newRotation) {
        transform->SetLocalRotation(Vector3(0, 0, newRotation));
    }
}

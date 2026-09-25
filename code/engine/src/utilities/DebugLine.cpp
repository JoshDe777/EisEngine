#include "engine/Game.h"
#include "engine/utilities/DebugLine.h"
#include "engine/components/rendering/renderers/LineRenderer.h"
#include "engine/components/rendering/meshes/Line.h"

namespace EisEngine {
    using Line = EisEngine::components::Line;
    using LineRenderer = EisEngine::components::LineRenderer;

    DebugLine::DebugLine(Game &engine, const Vector3 &startPoint, const Vector3 &endPoint, const Color& color) {
        this->engine = &engine;
        entity = &engine.entityManager.createEntity("DebugLine");
        transform = entity->transform;
        transform->SetLocalScale(Vector3::one);
        entity->AddComponent<LineRenderer>(color);
        //entity->GetComponent<LineRenderer>()->SetColor(color);
        entity->AddComponent<Line>(startPoint, endPoint);
    }

    void DebugLine::Invalidate() { engine->entityManager.deleteEntity(*entity);}

    void DebugLine::UpdateLinePosition(const Vector3 &startPoint, const Vector3 &endPoint)
    { entity->GetComponent<Line>()->SetPoints(startPoint, endPoint);}

    void DebugLine::UpdateColor(const EisEngine::Color &color)
    { entity->GetComponent<LineRenderer>()->SetColor(color);}
}

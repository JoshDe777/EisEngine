#include "MinotaursMaze.h"

MinotaursMaze::MinotaursMaze() : Game("Minotaur's Maze") {
    steve = make_shared<Steve>(*this);
    minotaur = make_shared<Minotaur>(*this);

    maze = make_shared<ProceduralMaze>(*this);

    camera.transform->SetLocalPosition(Vector3(0, 2, 0));

    controller = make_shared<CamController>(*this, minotaur->entity, steve->entity);

    RenderingSystem::MarkAsLoader(minotaur->entity.get());
    RenderingSystem::MarkAsLoader(steve->entity.get());
}

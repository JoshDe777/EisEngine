#pragma once

#include "engine/systems/SceneGraphUpdater.h"
#include "engine/Game.h"

namespace EisEngine::systems{

    SceneGraphUpdater::SceneGraphUpdater(Game &game) : System(game)
    { game.onUpdate.addListener([&] (Game &game){ UpdateTransforms(game);});}

    void SceneGraphUpdater::UpdateTransforms(EisEngine::Game &game) {
        if(!game.componentManager.hasComponentOfType<Transform>())
            return;
        game.componentManager.forEachComponent<Transform>([&] (Transform &transform){
            glm::mat4 oldMatrix = transform.modelMatrix;
            transform.modelMatrix = calculateModelMatrix(transform);
        });
    }

    glm::mat4 SceneGraphUpdater::calculateModelMatrix(EisEngine::components::Transform &transform) {
        if(transform.deleted)
            return glm::mat4(1.0f);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, (glm::vec3) transform.GetGlobalPosition());
        model = glm::rotate(model, glm::radians(transform.GetLocalRotation().z),
                            glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, (glm::vec3) transform.GetLocalScale());
        return model;
    }
}
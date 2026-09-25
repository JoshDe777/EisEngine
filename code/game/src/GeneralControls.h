#pragma once

#include "EisEngine.h"
#include "Scripts/GameManager.h"

namespace Ocean_Outlaws {
    using Camera = EisEngine::systems::Camera;

    class GeneralControls {
    public:
        explicit GeneralControls(Game &engine) :
        engine(engine) {
            camera = &engine.camera;
            camera->SetZoom(defaultZoom);
            engine.onUpdate.addListener([&] (Game &game){ ListenForControls();});
            gm = GameObject::FindObjectOfType<GameManager>();
            gm->onGameOver.addListener([&] (GameManager &gameManager){
                camera->transform->SetGlobalPosition(Vector3::zero);
                camera->SetZoom(defaultZoom);
            });
        }

    private:
        Game& engine;
        Camera *camera = nullptr;
        float cameraZoomSpeed = 0.3f;
        float defaultZoom = 0.03f;

        GameManager* gm = nullptr;

        void ListenForControls(){
            if(!gm)
                DEBUG_RUNTIME_ERROR("No Game Manager found!")

            if(gm->IsInGame()){
                // zoom out
                if (Input::GetKeyDown(KeyCode::LeftShift) || Input::GetKeyDown(KeyCode::RightShift))
                    camera->Zoom(-Time::deltaTime * cameraZoomSpeed);
                // zoom in
                if (Input::GetKeyDown(KeyCode::LeftControl) || Input::GetKeyDown(KeyCode::RightControl))
                    camera->Zoom(Time::deltaTime * cameraZoomSpeed);
            }
            else {
                if(Input::GetKeyDown(KeyCode::Enter))
                    gm->NavigateMenu();
                if(Input::GetKeyDown(KeyCode::Escape))
                    engine.Quit();
            }
        }
    };
}

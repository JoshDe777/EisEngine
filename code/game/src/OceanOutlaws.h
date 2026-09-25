#pragma once

#include <EisEngine.h>

#include "GameObjects/GameManagerObject.h"
#include "GameObjects/PlayerShip.h"
#include "GameObjects/EnemyShip.h"
#include "GeneralControls.h"

using namespace EisEngine;
namespace Ocean_Outlaws {
    class OceanOutlaws : public Game {
    public:
        explicit OceanOutlaws(const std::string &title);
    private:
        GameManagerObject gameManager;
        GeneralControls cameraController;
    };
}

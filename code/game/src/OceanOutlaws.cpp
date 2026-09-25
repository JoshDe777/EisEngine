#include "OceanOutlaws.h"

namespace Ocean_Outlaws {
    OceanOutlaws::OceanOutlaws(const std::string &title) :
    Game(title),
    gameManager(*this),
    cameraController(*this) { }
}

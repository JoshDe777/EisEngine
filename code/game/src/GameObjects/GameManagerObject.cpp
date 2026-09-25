#include "GameManagerObject.h"
#include "../Scripts/GameManager.h"
#include "../Scripts/Wind.h"
#include "../Scripts/AudioManager.h"

namespace Ocean_Outlaws {
    GameManagerObject::GameManagerObject(EisEngine::Game &engine) :
    GameObject(engine, "GameManager", "GameManager") {
        AddComponent<GameManager>();
        AddComponent<MapGenerator>();
        AddComponent<Wind>();
        AddComponent<AudioManager>();
    }
}

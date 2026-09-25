#pragma once

#include "../GameObject.h"

namespace Ocean_Outlaws {
    /// \n The central entity controlling the game's happenings.
    class GameManagerObject : public GameObject {
    public:
        /// \n Creates a new game manager object.
        explicit GameManagerObject(Game &engine);
    };
}

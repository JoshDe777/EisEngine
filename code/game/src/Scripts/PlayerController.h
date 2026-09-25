#pragma once

#include "EisEngine.h"

namespace Ocean_Outlaws {
    class PlayerShip;

    namespace scripts {
        class PlayerController : public Script {
        public:
            explicit PlayerController(Game& engine, guid_t owner, PlayerShip* playerShip);

            PlayerController(PlayerController&& other) noexcept;
        protected:
            void Update() override;
        private:
            PlayerShip* ship;
            Transform* transform = nullptr;

            void ManageSail();
            void RotateShip();
            void TryFiring();
        };
    }
}

using PlayerController = Ocean_Outlaws::scripts::PlayerController;

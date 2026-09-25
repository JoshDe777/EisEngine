#pragma once

#include "../GameObject.h"

namespace Ocean_Outlaws {
    using handle_t = EisEngine::event_t::handle_t;

    /// \n A supplies barrel that can be collected to heal players
    class Barrel : public GameObject {
    public:
        /// \n Creates a new barrel at the given position.
        explicit Barrel(Game &engine, const Vector3& pos);
        /// \n Signals to the barrel that it has been collected, prompting deletion in the following frame.
        void SignalCollected();
        /// \n A function called once every frame.
        void Update() override;
        /// \n The healing value provided on collecting a barrel.
        int healValue = 20;
    protected:
        /// \n A method called when a GameObject is intentionally deleted.
        void Invalidate() override;
    private:
        /// \n Determines whether a barrel was already collected.
        bool collected = false;
        /// \n The handle to the engine's update event for removal on deletion
        handle_t updateHandle;
        /// \n The relative path from the assets folder to the barrel's sprite.
        const fs::path spritePath = "images/sprites/ships/barrel.png";
    };
}

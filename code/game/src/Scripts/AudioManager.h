#pragma once

#include <soloud.h>
#include <soloud_wav.h>
#include <EisEngine.h>

namespace Ocean_Outlaws {
    class AudioManager : public Script {
    public:
        explicit AudioManager(Game& engine, guid_t owner);
        void SetVolumeModifier(const float& mod){ speaker.setGlobalVolume(mod * Volume);}
    protected:
        void Start() override;
        void Update() override;
        void Invalidate() override;
    private:
        const float Volume = 0.5f;
        SoLoud::Soloud speaker;
        std::vector<SoLoud::Wav*> backgroundPlaylist = { };
        const std::vector<fs::path> playlistAssetPaths = {
                "audio/background/dread-pirate-roberts.mp3",
                "audio/background/fearsome-tipsy-pirates.mp3",
                "audio/background/pirate-adventures.mp3",
                "audio/background/winter-white.mp3"
        };
        int currentTrackIndex = 0;

        void PlayNextTrack();
    };
}

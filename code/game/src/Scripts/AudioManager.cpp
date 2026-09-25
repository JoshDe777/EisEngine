#include "AudioManager.h"

namespace Ocean_Outlaws {
    AudioManager::AudioManager(EisEngine::Game &engine, EisEngine::ecs::guid_t owner) : Script(engine, owner) { }

    void AudioManager::Start() {
        speaker.init();
        speaker.setGlobalVolume(0.5f * Volume);
        currentTrackIndex = RandomInt(0, (int) playlistAssetPaths.size() - 1);
        for(auto& path : playlistAssetPaths){
            auto* track = new SoLoud::Wav;
            track->load(resolveAssetPath(path).string().c_str());
            track->setSingleInstance(true);
            backgroundPlaylist.emplace_back(track);
        }
    }

    void AudioManager::Update() {
        if(speaker.getActiveVoiceCount() == 0)
            PlayNextTrack();
    }

    void AudioManager::Invalidate() {
        speaker.deinit();
        auto playlistCopy = backgroundPlaylist;
        for(auto track : playlistCopy)
            delete track;
        Script::Invalidate();
    }

    void AudioManager::PlayNextTrack() {
        speaker.stopAll();
        speaker.play(*backgroundPlaylist[currentTrackIndex]);
        currentTrackIndex = (currentTrackIndex + 1) % (int) backgroundPlaylist.size();
    }
}

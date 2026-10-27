#pragma once
#include <GCore.h>

#include <miniaudio/miniaudio.h>
#include <iostream>
#include <unordered_map>
#include <memory>

class AudioEngine {
public:
    AudioEngine();
    ~AudioEngine();

    void init();

    void oneHit(const std::string&);                                // plays a sound once
    gUint startAudio(const std::string& path, bool loop);    // start audio loop
    void stopAudio(gUint id);                                // stop an audio by id
    void setVolume(gUint id, float volume);                  // set volume of sound
    void pan(gUint id, float pan);

    void tick();
    void end();

private:
    ma_engine soundEngine;
    std::unordered_map<gUint, std::unique_ptr<ma_sound>> sounds;
    gUint index;
};
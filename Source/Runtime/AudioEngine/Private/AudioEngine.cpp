#include "AudioEngine.h"

AudioEngine::AudioEngine() : index(0) {}
AudioEngine::~AudioEngine() {}

void AudioEngine::init() {
    ma_result res;
    res = ma_engine_init(NULL, &soundEngine);

    if(res != MA_SUCCESS) {
        std::cout << "[WARNING] Failed to initialize Audio Manager!\n";
        return;
    }
}

void AudioEngine::oneHit(const std::string &path) {
    ma_engine_play_sound(&soundEngine, path.c_str(), NULL);
}

gUint AudioEngine::startAudio(const std::string& path, bool loop) {
    gUint rID = index++;
    auto sound = std::make_unique<ma_sound>();
    ma_result res = ma_sound_init_from_file(&soundEngine, path.c_str(), 0, nullptr, nullptr, sound.get());
    if(res != MA_SUCCESS) {
        std::cout << "[WARNING] failed to initialize Audio from file " << path << std::endl;
        return 0;
    }

    ma_sound_set_looping(sound.get(), loop);
    ma_sound_start(sound.get());

    sounds[index] = std::move(sound);

    return rID;
}

void AudioEngine::stopAudio(gUint id) {
    auto it = sounds.find(id);
    if(it == sounds.end()) return;

    ma_sound_stop(it->second.get());
    ma_sound_uninit(it->second.get());
    sounds.erase(it);   
}

void AudioEngine::setVolume(gUint id, float volume) {
    auto it = sounds.find(id);
    if(it != sounds.end()) {
        ma_sound_set_volume(it->second.get(), volume);
    }
}

void AudioEngine::pan(gUint id, float pan) {
    auto it = sounds.find(id);
    if(it != sounds.end()) ma_sound_set_pan(it->second.get(), pan);
}

void AudioEngine::tick() {
    for(auto it = sounds.begin(); it != sounds.end(); ) {
        if(ma_sound_at_end(it->second.get())) {
            ma_sound_uninit(it->second.get());
            sounds.erase(it);
            continue;
        }
        ++it;
    }
}

void AudioEngine::end() {
    for(auto it = sounds.begin(); it != sounds.end();) {
        ma_sound_stop(it->second.get());
        ma_sound_uninit(it->second.get());
        it = sounds.erase(it);
    } 
}
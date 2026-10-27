#include "Time.h"
#include <chrono>
#include <unordered_map>

float Time::deltaTime = 0.f;
gUint Time::fps = 0.f;
std::unordered_map<gUint, gTimer> Time::timers = {};
std::unordered_map<gUint, gStopwatch> Time::stopwatches = {};

void Time::setDelta(float dT) { deltaTime = dT; }

gUint Time::startTimer(float duration) {
    gTimer result;
    result.duration = std::chrono::duration<float>(duration);
    result.start = steady_c::now();
    result.done = false;
    result.free = false;

    gUint id = gGenerateID();
    timers[id] = result;

    return id;
}

gTimer* Time::getTimer(gUint id) {
    auto it = timers.find(id);
    if(it != timers.end()) return &timers[id];
    return nullptr;
}

gUint Time::startStopwatch() {
    gStopwatch result;
    result.start = steady_c::now();
    result.duration = std::chrono::duration<float>::zero(); // Start stopwatch at 0
    result.done = false;
    result.free = false;

    gUint id = gGenerateID();
    stopwatches[id] = result;
    return id;
}

gStopwatch* Time::getStopwatch(gUint id) {
    auto it = stopwatches.find(id);
    if(it != stopwatches.end()) return &stopwatches[id];
    return nullptr;
}

float Time::elapsed(gTimer* timer) {
    if(timer) return std::chrono::duration<float>(steady_c::now() - timer->start).count();
    return 0.0f;
}

void Time::tick() {
    for(auto it = timers.begin(); it != timers.end(); ) {
        if(it->second.free) { 
            it = timers.erase(it);
            continue;
        }
        // Time comparison of doom and despair
        if(std::chrono::duration_cast<std::chrono::seconds>(steady_c::now() - it->second.start) == it->second.duration) {
            it->second.done = true;
        }
    }
    for(auto it = stopwatches.begin(); it != stopwatches.end(); ) {
        if(it->second.free) {
            it = stopwatches.erase(it);
            continue;
        }
        it->second.duration = steady_c::now() - it->second.start;
    }
}
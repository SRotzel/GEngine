#pragma once

#include <chrono>
#include <unordered_map>
#include <vector>
#include "../gEngineCore.h"

using steady_c = std::chrono::steady_clock;

// Give a certain time, timer is done when that time is reached
typedef struct {
    steady_c::time_point start;
    std::chrono::duration<float> duration;
    bool done, free;
} gTimer;

typedef struct {
    steady_c::time_point start;
    std::chrono::duration<float> duration;
    bool done, free;
} gStopwatch;

class Time {
public:
    // Should not be normally called by user
    static void setDelta(float); 

    static gUint startTimer(float duration);
    static gTimer* getTimer(gUint id);

    static gUint startStopwatch();
    static gStopwatch* getStopwatch(gUint id);
    static void tick();

    static float elapsed(gTimer* timer);

    static float deltaTime;
    static gUint fps;
private:
    static std::unordered_map<gUint, gTimer> timers;
    static std::unordered_map<gUint, gStopwatch> stopwatches;
};
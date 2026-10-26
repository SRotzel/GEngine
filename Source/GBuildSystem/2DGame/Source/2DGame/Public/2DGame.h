#pragma once
#include <Engine/Engine.h>


class 2DGame {
    2DGame();

    virtual void start() {}
    virtual void tick()  {}
    virtual void end()   {}
}
#pragma once
#include <Engine/Engine.h>


class 2DCharacter {
    2DCharacter();

    virtual void start() {}
    virtual void tick()  {}
    virtual void end()   {}
}
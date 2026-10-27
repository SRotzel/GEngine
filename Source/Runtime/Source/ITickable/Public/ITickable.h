#pragma once

class ITickable {
    virtual ~ITickable() = default;

    virtual void start() {}
    virtual void tick()  {}
    virtual void end()   {}
};
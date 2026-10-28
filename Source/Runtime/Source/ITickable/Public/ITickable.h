#pragma once

class ITickable {
public:
    virtual ~ITickable() = default;

    virtual void start() {}
    virtual void tick()  {}
    virtual void end()   {}

    inline bool isActive() const { return this->active; }
    inline void setActive(bool v) { this->active = v; }
    inline void toggle();

private:
    bool active = true;
};
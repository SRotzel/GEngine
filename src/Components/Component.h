#pragma once
#include <imgui.h>
#include "../gEngineCore.h"
class gEntity;

class Component {
public:
    Component(unsigned int id = 0) : id(id), active(true) {}
    virtual ~Component() {}

    virtual void start() {}
    virtual void tick() {}
    virtual void end() {}

    virtual void onDebug() {}

    inline gEntity* getOwner() const { return owner; }
    inline void setOwner(gEntity* parent) { owner = parent; }
    inline gUint getID() const { return id; }

    inline bool isActive() { return active; }
    inline void setActive(bool a) { active = a; }
    inline void toggle() { active = !active; }

protected:
    bool active;
    using Super = Component;
    gEntity *owner = nullptr;
    gUint id;
};
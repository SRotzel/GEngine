#pragma once

#include "GCore.h"

#include "imgui/imgui.h"

class gEntity;

class Component : public ITickable {
public:
    Component(unsigned int id = 0) : id(id), active(true) {}
    virtual ~Component() {}

    virtual void onDebug() {}

    inline gEntity* getOwner() const { return owner; }
    inline void setOwner(gEntity* parent) { owner = parent; }
    inline gUint getID() const { return id; }

protected:
    using Super = Component;
    gEntity *owner = nullptr;
    gUint id;
};
#pragma once
#include "Scene.h"
#include "GCore.h"

class GEngine;

class GApplication : public ITickable {
public:
    GApplication();
    virtual ~GApplication();

    void start() override;
    void tick()  override;
    void end()   override;

    // called during debug draw calls
    virtual void onDebug();

    void loadScene(gScene*);

    gScene *activeScene = nullptr;
protected:
    friend GEngine;
    using Super = GApplication;
};
#pragma once
#include <memory>
#include <vector> 
#include <unordered_map>

#include "Camera.h"
#include "Entity.h"
#include "gEngineCore.h"

class gScene {
public: 
    gScene(std::vector<std::unique_ptr<gEntity>> entities);
    gScene();
    ~gScene();

    gScene(const gScene&) = delete;
    gScene& operator=(const gScene&) = delete;
    gScene(gScene&&) = default;
    gScene& operator=(gScene&&) = default;

    gEntity* addEntity(std::unique_ptr<gEntity>);
    std::vector<std::unique_ptr<gEntity>> &getEntities();
    void removeEntity(gUint id);

    void setActiveCam(gUint id);
    gCamera* getActiveCam() const;

    void setSkybox(Vector4 c);
    Vector4 getSkybox();

    virtual void start();
    virtual void tick();
    virtual void end();

protected:
    gCamera *activeCamera = nullptr;

    std::vector<std::unique_ptr<gEntity>> entities;
    Vector4 skyboxColor; // Background color
};
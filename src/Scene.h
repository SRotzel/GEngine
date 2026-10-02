#pragma once
#include <memory>
#include <vector> 
#include <unordered_map>
#include "Entity.h"
#include "gEngineCore.h"

class gScene {
public: 
    gScene(std::vector<std::unique_ptr<gEntity>> entities);
    gScene();
    virtual ~gScene();

    gScene(const gScene&) = delete;
    gScene& operator=(const gScene&) = delete;
    gScene(gScene&&) = default;
    gScene& operator=(gScene&&) = default;

    void addEntity(std::unique_ptr<gEntity>);
    std::vector<std::unique_ptr<gEntity>> &getEntities();

    virtual void start();
    virtual void tick();
    virtual void end();

    void setSkybox(Vector4 c);
    Vector4 getSkybox();

protected:
    std::vector<std::unique_ptr<gEntity>> entities;
    Vector4 skyboxColor; // Background color
};
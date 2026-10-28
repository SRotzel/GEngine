#pragma once
#include "Scene.h"
#include "GCore.h"

class GApplication : public ITickable {
public:
    GApplication() {}
    virtual ~GApplication() {}

    virtual void start() {
        gScene* scene = new gScene();
        loadScene(scene); 
    }
    virtual void tick() {
        if(activeScene) activeScene->tick();
    }
    virtual void end() {}

    // called during debug draw calls
    virtual void onDebug() {
        for(const auto& e : activeScene->getEntities()) {
            // Dev Debug gui
            std::string label = "Entity " + std::to_string(e->id);
            if(ImGui::CollapsingHeader(label.c_str())) {
                Transform &t = e->transform;
                ImGui::DragFloat2("position", &t.position.x, 1.f);
                ImGui::DragFloat("rotation", &t.rotation, 1.f);
                ImGui::DragFloat2("size", &t.size.x, 0.1f);
                ImGui::Separator();
                for(auto&& c : e->getComponents()) c->onDebug();
            }
        }
    } 

    void loadScene(gScene*);

protected:
    friend gEngine;
    using Super = GApplication;
    gScene *activeScene = nullptr;
};
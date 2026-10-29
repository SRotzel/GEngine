#include "GApplication.h"

GApplication::GApplication() {}
GApplication::~GApplication() {}

void GApplication::start() {
    gScene* scene = new gScene();
    loadScene(scene); 
}

void GApplication::tick() {
    if(activeScene) activeScene->tick();
}

void GApplication::end() {}

void GApplication::onDebug() {
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

// TODO: refactor
void GApplication::loadScene(gScene *s) {
    if(s == nullptr) {
        std::cout << "Attempting to load invalid Scene!\n";
        return;
    }
    
    if(activeScene) {
        activeScene->end();
        delete activeScene;
        activeScene = s;
        activeScene->start();
        return;
    }

    activeScene = s;
    activeScene->start();
}
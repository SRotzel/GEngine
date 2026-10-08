#include "Scene.h"
#include "Camera.h"
#include "Entity.h"
#include <memory>

gScene::gScene(std::vector<std::unique_ptr<gEntity>> ent) : entities(std::move(ent)), skyboxColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f)) { }
gScene::gScene() {}

gScene::~gScene() {  }

gEntity* gScene::addEntity(std::unique_ptr<gEntity> ent) { 
    gEntity* ptr = ent.get();
    entities.push_back(std::move(ent)); 
    return ptr;
}
std::vector<std::unique_ptr<gEntity>>& gScene::getEntities() { return entities; }

void gScene::removeEntity(gUint id) {
    for(auto it = entities.begin(); it != entities.end(); )  {
        if(it->get()->id != id) { 
            ++it;
            continue; 
        }
        it->get()->end();
        entities.erase(it);
        std::cout << "Deleted entity " << id << " from Scene Scene.cpp\n ";
    }
}

void gScene::setActiveCam(gUint id) { 
    std::cout << "trying to activate camera " << id << std::endl;
    /*for(const auto& e : entities) {
        std::cout << e->id << std::endl;
        if(e->id != id) continue;
        if(e.get()) activeCamera = dynamic_cast<gCamera*>(e.get()); // todo: refactor all this bs, bad practice
        else activeCamera = nullptr;
    }*/
    for(auto it = entities.begin(); it != entities.end(); ++it) {
        if(it->get()->id != id) continue;
        gCamera *temp = dynamic_cast<gCamera*>(it->get());
        if(temp) activeCamera = temp;
    }
}
gCamera* gScene::getActiveCam() const { return activeCamera; }

void gScene::setSkybox(Vector4 c) { this->skyboxColor = c; }
Vector4 gScene::getSkybox()       { return skyboxColor; }

void gScene::start() {

    for(const auto& e : entities) {
        e->start();
    }
}

void gScene::tick() {
    for(const auto& e : entities) {
        e->tick();
    }
}

void gScene::end() {
    for(const auto& e : entities) {
        e->end();
    }
}
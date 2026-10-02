#include "Scene.h"
#include "Entity.h"

gScene::gScene(std::vector<std::unique_ptr<gEntity>> ent) : entities(std::move(ent)), skyboxColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f)) {}
gScene::gScene() {}

gScene::~gScene() {  }

std::vector<std::unique_ptr<gEntity>>& gScene::getEntities() {
    return entities;
}

void gScene::addEntity(std::unique_ptr<gEntity> ent) {
    entities.push_back(std::move(ent));
}

void gScene::removeEntity(gUint id) {
    for(const auto it = entities.begin(); it != entities.begin(); )  {
        if(it->get()->id != id) continue;
        it->get()->end();
        entities.erase(it);
    }
}

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

void gScene::setSkybox(Vector4 c) { this->skyboxColor = c; }
Vector4 gScene::getSkybox()       { return skyboxColor; }
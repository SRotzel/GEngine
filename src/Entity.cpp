#include "Entity.h"
#include "Components/Component.h"
#include "gEngineCore.h"
#include <memory>

gEntity::gEntity(gScene* parent) : owner(parent), id(gGenerateID()), active(true) {}
gEntity::~gEntity() {}


void gEntity::start() {
    for(auto& c : components) {
        c->start();
    }
}

void gEntity::tick() {
    for(auto& c : components) {
        c->tick();
    }
}

void gEntity::end() {
    for(auto& c : components) {
        c->end();
    }
}

void gEntity::attachComponent(std::unique_ptr<Component> c) {
    if(c->getID() == 0) {
        std::cout << "Invalid component ID " << c->getID() << std::endl;
        return;
    }
    c->setOwner(this);
    components.push_back(std::move(c));
}

void gEntity::detachComponent(unsigned int cID) {
    for (auto it = components.begin(); it != components.end(); ) {
        if ((*it)->getID() == cID) {
            (*it)->end();
            it = components.erase(it);   // erase() returns the next valid iterator
        } else {
            ++it;
        }
    }
}

std::vector<std::unique_ptr<Component>> &gEntity::getComponents() {
    return components;
}
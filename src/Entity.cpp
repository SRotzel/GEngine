#include "Entity.h"

gEntity::gEntity(gScene* parent) : owner(parent), id(gGenerateID()), active(true) {
    transform.rotation = 0.0f;
    transform.size = Vector2(100, 100);
    transform.position = Vector2(0,0);
}

gEntity::gEntity(gScene* parent, const std::string &name) : owner(parent), id(gGenerateID()), active(true) {
    transform.rotation = 0.0f;
    transform.size = Vector2(100, 100);
    transform.position = Vector2(0,0);
}
gEntity::~gEntity() {}

void gEntity::start() {
    for(auto& c : components) {
        c->start();
    }
}

void gEntity::tick() {
    if(transform.rotation > 360 || transform.rotation < -360) transform.rotation = 0;

    if(!this->active) return;
    for(auto& c : components) {
        c->tick();
    }
}

void gEntity::end() {
    for(auto& c : components) {
        c->end();
    }
}

void gEntity::rename(const std::string &name) { this->name = name; }
const std::string& gEntity::getName() const { return name; }

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
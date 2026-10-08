#pragma once
#include "Components/ColliderComponent.h"
#include "Components/PhysicsComponent.h"
#include "Components/TextureComponent.h"
#include "gEngineCore.h"
#include "Components/Component.h"
#include <memory>
#include <vector>
#include <iostream>

class gScene;

class gEntity {
public:
    gEntity(gScene* parent);
    virtual ~gEntity();

    gEntity(const gEntity&) = delete;
    gEntity& operator=(const gEntity&) = delete;
    gEntity(gEntity&&) = default;
    gEntity& operator=(gEntity&&) = default;

    virtual void start();
    virtual void tick();
    virtual void end();

    void attachComponent(std::unique_ptr<Component>);

    template <typename T, typename... Args> 
    T* attachComponent(Args&&...args) {
        auto comp = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr    = comp.get();
        attachComponent(std::move(comp));
        return ptr;
    }

    void detachComponent(unsigned int);

    template <typename T> 
    T* getComponent() const {
        for(auto &c : components) {
            if(T* match = dynamic_cast<T*>(c.get())) return match;
        }
        return nullptr;
    }

    template <typename T>
    T* getActiveComponent() const {
        for(auto &c : components) {
            if(T* match = dynamic_cast<T*>(c.get())) {
                if(match->isActive()) return match;
            }
        }
        return nullptr;
    }
    std::vector<std::unique_ptr<Component>> &getComponents();

    inline PhysicsComponent& cphysics() { return *getComponent<PhysicsComponent>(); }
    inline TextureComponent& ctexture() { return *getComponent<TextureComponent>(); }
    inline AABBColliderComponent& ccollider() { return *getComponent<AABBColliderComponent>(); }
    inline bool isActive() const { return this->active; }
    inline void setActive(bool v) { this->active = v; }
    inline void toggle() { active = !active; }

    Transform transform;
    gUint id;

protected:
    using Super = gEntity;  
    bool active;
    std::string name; // TODO

private:
    gScene *owner;
    std::vector<std::unique_ptr<Component>> components;
};
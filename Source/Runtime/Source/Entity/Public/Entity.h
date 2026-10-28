#pragma once

/*Engine Headers*/
#include "Component.h"
#include "ColliderComponent.h"
#include "PhysicsComponent.h"
#include "TextureComponent.h"
#include "GCore.h"

#include <memory>
#include <vector>
#include <iostream>

class gScene;

class gEntity : public ITickable {
public:
    gEntity(gScene* parent);
    virtual ~gEntity();

    gEntity(const gEntity&) = delete;
    gEntity& operator=(const gEntity&) = delete;
    gEntity(gEntity&&) = default;
    gEntity& operator=(gEntity&&) = default;

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

    Transform transform;
    gUint id;

protected: using Super = gEntity;
private:
    gScene *owner;
    std::vector<std::unique_ptr<Component>> components;
};
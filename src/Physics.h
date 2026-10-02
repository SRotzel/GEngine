#pragma once

#include <memory>
#include <cmath>
#include "Scene.h"
#include "Components/ColliderComponent.h"
#include "Components/PhysicsComponent.h"
#include "Systems/Time.h"

constexpr float gGravity = -9.81f; // objects fall down, hence negative
using std::unique_ptr;

typedef struct {
    Vector2 normal;
    float time; // normalized between 0 and 1
    bool collide;
} Collision;

// Should generally be owned only by gEngine
class PhysicsSystem {
public:
    PhysicsSystem(gScene* s);
    PhysicsSystem();
    ~PhysicsSystem();

    void init(gScene* s);

    void physicsTick();
    void setScene();
    // Check every entity against every entity in the function -> set other participants as collided 
    
private:
    void AABBCollCheck(gEntity*, AABBColliderComponent*);
    Collision sweep(AABBColliderComponent* moving, Vector2 movement, AABBColliderComponent* target);
    gScene* scene;
};
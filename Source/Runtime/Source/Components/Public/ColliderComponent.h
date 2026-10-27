#pragma once
#include "Component.h"
#include <vector>

class AABBColliderComponent;
typedef struct {
    bool collides; 
    std::vector<AABBColliderComponent*> participants;
    // TODO: depth
} CollisionInfo;

// gEngine Component to make an Entity collidable using the AABB algorithm.
// Params: 
// Transform t. no default. preferrably enter Transform of entity or seperate Transform for your Collider.
// unsigned int id. default = 0. Unique identifier to your component. Tip: use gGenerateID() for safe IDs.
class AABBColliderComponent : public Component {
public:
    AABBColliderComponent(Transform t, unsigned int id);
    
    void updateBounds(Transform t);
    void tick() override;
    void onDebug() override;

    inline bool collides() const { return collision.collides; }

    // Format: MinX, MinY, MaxX, MaxY
    Vector2 min{1.f, 1.f}, max{1.f, 1.f};
    CollisionInfo collision;
    bool render;
};

// not finished yet
class SATColliderComponent : public Component {
    
};

bool AABBCollide(const AABBColliderComponent& a, const AABBColliderComponent& b);
#include "Physics.h"
#include "Components/ColliderComponent.h"
#include "Components/PhysicsComponent.h"
#include "gEngineCore.h"

PhysicsSystem::PhysicsSystem(gScene* s) {
    scene = s;
}
PhysicsSystem::PhysicsSystem() {}
PhysicsSystem::~PhysicsSystem() {}

void PhysicsSystem::init(gScene* s) { scene = s; }

// tell other component it collides also for efficiency, and skip if it is flagged as collided already X
// optimize
void PhysicsSystem::AABBCollCheck(gEntity* e1, AABBColliderComponent* a) {
    if(!a) {
        a->collision.collides = false;
        return;
    }
    bool result = false;

    for(const auto& e2 : scene->getEntities()) {
        if(e2->id == e1->id) continue; // Skip if it's the same entity
        if(AABBColliderComponent* b = e2->getComponent<AABBColliderComponent>()) {
            if(AABBCollide(*a, *b)) {
                result = true;
                a->collision.participants.push_back(b);
                b->collision.participants.push_back(a);
            }
        }
    }
    a->collision.collides = result;
}

Collision PhysicsSystem::sweep(AABBColliderComponent* moving, Vector2 movement, AABBColliderComponent* target) {
    Collision result;
    result.collide = false;
    result.time = 0;
    result.normal = v2zero();
    if (movement == v2zero()) {
        return result;
    }

    Vector2 centerA = moving->getOwner()->transform.position;
    Vector2 halfsize = (moving->max - moving->min) * 0.5f;

    Vector2 expandedMin = target->min - halfsize;
    Vector2 expandedMax = target->max + halfsize;

    float timeX1 = (expandedMin.x - centerA.x) / movement.x; // if a is moving right
    float timeX2 = (expandedMax.x - centerA.x) / movement.x; // if a is moving left
    if(timeX1 > timeX2) std::swap(timeX1, timeX2);

    float timeY1 = (expandedMin.y - centerA.y) / movement.y;
    float timeY2 = (expandedMax.y - centerA.y) / movement.y;
    if(timeY1 > timeY2) std::swap(timeY1, timeY2);

    float enterTime = std::max(timeX1, timeY1);
    float exitTime  = std::min(timeX2, timeY2);
    
    if(enterTime >= 0.0f && enterTime <= 1.0f) {
        result.collide = true;
        result.time = enterTime;
        return result;
    }

    return result;
}

void PhysicsSystem::physicsTick() {
    for(const auto& e : scene->getEntities()) {
        if(auto pc = e->getActiveComponent<PhysicsComponent>()) {
            pc->force.y += pc->mass * gGravity; 
            pc->velocity += (pc->force / pc->mass) * Time::deltaTime;

            /*float terminalVelocity = std::sqrt((2 * pc->mass * gGravity) / (pc->getOwner()->transform.size.x * pc->getOwner()->transform.position.y)); // TODO: refactor
            if(pc->velocity.y > terminalVelocity) pc->velocity.y = terminalVelocity; */
            Vector2 actualMovement = pc->velocity * Time::deltaTime;

            if(auto cc = e->getActiveComponent<AABBColliderComponent>()) {
                cc->updateBounds(e->transform);
                AABBCollCheck(e.get(), cc);

                Collision closest;
                closest.collide = false;
                for(const auto &e2 : scene->getEntities()) {
                    AABBColliderComponent* cc2 = e2->getActiveComponent<AABBColliderComponent>();
                    if(e2->id == e->id || cc2 == nullptr) continue;

                    Collision collision = sweep(cc, actualMovement, cc2);
                    if((!closest.collide || collision.time < closest.time) && collision.collide) {
                        closest = collision;
                    }
                }

                if(closest.collide) {
                    actualMovement *= closest.time;
                }
            }
            if(actualMovement == v2zero()) pc->velocity = v2zero();
            pc->force = v2zero();
            pc->getOwner()->transform.position += actualMovement;
            continue;
        }
        if(auto c = e->getActiveComponent<AABBColliderComponent>()) {
            c->updateBounds(e->transform);
            AABBCollCheck(e.get(), c);
        }
    }
}
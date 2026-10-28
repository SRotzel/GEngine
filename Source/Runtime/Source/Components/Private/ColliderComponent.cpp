#include "ColliderComponent.h"

AABBColliderComponent::AABBColliderComponent(Transform t, unsigned int id) 
        : Component(id) {
    updateBounds(t);
    collision = {};
    render = false;
}

void AABBColliderComponent::updateBounds(Transform t) {
    if(owner == nullptr) return;
    float minX, minY, maxX, maxY;
    Vector2 halfSize = t.size / 2.f;

    minX = t.position.x - halfSize.x;
    minY = t.position.y - halfSize.y;
    maxX = t.position.x + halfSize.x;
    maxY = t.position.y + halfSize.y;
    min = {minX, minY};
    max = {maxX, maxY};
}

void AABBColliderComponent::tick() {
    if(!collision.collides) collision.participants.clear();
}

void AABBColliderComponent::onDebug() {
    ImGui::Text("Collider Component");
    ImGui::Checkbox("active", &active);
    ImGui::Value("Left", min.x);
    ImGui::Value("Right", max.x);
    ImGui::Value("Bottom", min.y);
    ImGui::Value("Top", max.y);
    ImGui::Value("collision?", collision.collides);
    ImGui::Separator();
}

bool AABBCollide(const AABBColliderComponent& a, const AABBColliderComponent& b) {
    return a.min.x <= b.max.x && a.max.x >= b.min.x &&
           a.min.y <= b.max.y && a.max.y >= b.min.y;
}
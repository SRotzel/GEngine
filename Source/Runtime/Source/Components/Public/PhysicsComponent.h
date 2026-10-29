#pragma once
#include "Component.h"
#include "Time.h"

class PhysicsComponent : public Component {
public:
    PhysicsComponent(gUint id) : Component(id) {
        mass = 1.0f;
        friction = 0.5f;
        velocity = Vector3(0.f, 0.f, 0.f);
    }

    void onDebug() override {
        ImGui::Text("Physics Component");
        bool active = isActive();
        ImGui::Checkbox("active", &active);
        ImGui::SliderFloat("mass", &mass, 0.0f, 100.f);
        ImGui::SliderFloat("friction", &friction, 0.0f, 1.0f);
        ImGui::DragFloat2("force: ", &force.x, 0.1f);
        ImGui::DragFloat2("velocity", &velocity.x, 0.1f);
        ImGui::Separator();
    }

    void addForce(Vector2 add) {
        force += add * Time::deltaTime;
    }

    float mass;
    float friction; // Normalized to 0.0 - 1.0 (not implemented yet)
    Vector2 velocity, force;
};
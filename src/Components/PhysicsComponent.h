#pragma once
#include "Component.h"
#include "../Systems/Time.h"
#include "../gEngineCore.h"
#include <imgui.h>

class PhysicsComponent : public Component {
public:
    PhysicsComponent(gUint id) : Component() {
        mass = 1.0f;
        friction = 0.5f;
        velocity = Vector2(0.f, 0.f);
        force = Vector2(0.f, 0.f);
    }

    void tick() override {
        
    }

    void onDebug() override {
        ImGui::Text("Physics Component");
        ImGui::Checkbox("active", &active);
        ImGui::SliderFloat("mass", &mass, 0.0f, 1000.f);
        ImGui::SliderFloat("friction", &friction, 0.0f, 1.0f);
        ImGui::DragFloat2("force: ", &force.x, 0.1f);
        ImGui::DragFloat2("velocity", &velocity.x, 0.1f);
        ImGui::Separator();
    }

    void addForce(Vector2 add) { force += add; }
    void setVelocity(Vector2 vel) { velocity = vel; }
    void addImpulse(Vector2 impulse) { 
        Vector2 deltaV = impulse / mass;
        velocity += deltaV;
    }
    
    float mass;
    float friction; // Normalized to 0.0 - 1.0 (not implemented yet)
    Vector2 velocity, force;
};
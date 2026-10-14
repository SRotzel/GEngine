#include "Player.h"

void gPlayer::tick() {
    Super::tick();
	if(InputManager::isKeyHeld(GLFW_KEY_D)) { 
		if(const auto& physics = getActiveComponent<PhysicsComponent>()) {
			physics->addImpulse({1000.f, 0});
		}
	} 
	if(InputManager::isKeyHeld(GLFW_KEY_A)) { 
		if(const auto& physics = getActiveComponent<PhysicsComponent>()) {
			physics->addImpulse({-10000.f, 0});
		}
	}
	if(InputManager::isKeyPressed(GLFW_KEY_SPACE)) {
		if(const auto& physics = getActiveComponent<PhysicsComponent>()) {
			physics->addImpulse({0.f, 1000});
		}
	}
}
#pragma once
#include "Entity.h"
#include "Systems/Input.h"
#include "Systems/Time.h"

class gPlayer : public gEntity {
public:
	gPlayer(gScene* s) : gEntity(s) {}

	void tick() override {
		if(InputManager::isKeyHeld(GLFW_KEY_D)) { 
			transform.position.x += move * Time::deltaTime;
			if(ccollider().collides()) transform.position.x -=move * Time::deltaTime;
		} 
		if(InputManager::isKeyHeld(GLFW_KEY_A)) { 
			transform.position.x -= move * Time::deltaTime;
			if(ccollider().collides()) transform.position.x +=move * Time::deltaTime;
		}
		if(InputManager::isKeyHeld(GLFW_KEY_W)) { 
			transform.position.y += move * Time::deltaTime;
			if(ccollider().collides()) transform.position.y -=move * Time::deltaTime;
		}
		if(InputManager::isKeyHeld(GLFW_KEY_S)) { 
			transform.position.y -= move * Time::deltaTime;
			if(ccollider().collides()) transform.position.y +=move * Time::deltaTime;
		}
		if(InputManager::isKeyPressed(GLFW_MOUSE_BUTTON_1)) std::cout << "Mouse at: " << InputManager::getMousePos().x << InputManager::getMousePos().y << std::endl;
	}
private:
	float move = 100.f;
};
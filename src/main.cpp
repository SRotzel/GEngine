#include "gEngine.h"
#include "gEngineCore.h"

#include "Entity.h"
#include "Player.h"
#include "Systems/Audio.h"
#include "Components/ColliderComponent.h"

class Game : public gApplication {
public:
	Game() : gApplication() {}
	
	void start() override {
		Super::start();
		
		std::unique_ptr<gEntity> floor = std::make_unique<gPlayer>(activeScene);
		//player->attachComponent<TextureComponent>("assets/cursor2.png", gGenerateID());
		floor->attachComponent<AABBColliderComponent>(floor->transform, gGenerateID());
		floor->ccollider().render = true;
		floor->transform.position = Vector2(0.f, -365.f);
		floor->transform.size.x = 1500;
		
		std::unique_ptr<gEntity> object = std::make_unique<gEntity>(activeScene);
		object->attachComponent<PhysicsComponent>(gGenerateID());
		object->cphysics().setActive(false);
		object->attachComponent<AABBColliderComponent>(object->transform, gGenerateID());

		std::unique_ptr<gEntity> wall = std::make_unique<gEntity>(activeScene);
		wall->transform.position = Vector2(-500, 0);
		wall->transform.size 	 = Vector2(100, 1000);
		wall->attachComponent<AABBColliderComponent>(wall->transform, gGenerateID());
		
		activeScene->addEntity(std::move(floor));
		activeScene->addEntity(std::move(object));
		activeScene->addEntity(std::move(wall));
		activeScene->setSkybox(Vector4(1.0f, 0.7f, 0.4f, 1.0f));
		
		audio.init();
		audio.startAudio("assets/music.wav", true);
	}
	
	void tick() override {
		Super::tick();
		audio.tick();

		Vector4 color = activeScene->getSkybox();
		if(color.x >= 1.0f) increment = -0.005f;
		if(color.x <= 0.0f) increment = 0.005f;
		color.x += increment;
		activeScene->setSkybox(Vector4(color));
	}
	
	void end() override {
		Super::end();
		activeScene->end();
		audio.end();
	}

private:
	AudioEngine audio;
	float increment = 0.005f;
};

#define G_ENTRY_POINT Game
#define G_ENGINE_MAIN
#include "gEngineMain.h"
#include "gEngine.h"
#include "gEngineCore.h"

#include "Entity.h"
#include "Player.h"
#include "Camera.h"
#include "Systems/Audio.h"
#include "Components/ColliderComponent.h"
#include <memory>

class Game : public gApplication {
public:
	Game() : gApplication() {}
	
	void start() override {
		Super::start();

		auto camera = gInst<gCamera>(activeScene);
		
		auto floor = gInst<gPlayer>(activeScene);
		//player->attachComponent<TextureComponent>("assets/cursor2.png", gGenerateID());
		floor->attachComponent<AABBColliderComponent>(floor->transform, gGenerateID());
		floor->ccollider().render = true;
		floor->transform.position = Vector2(0.f, -365.f);
		floor->transform.size.x = 1500;
		
		auto player = gInst<gPlayer>(activeScene);
		player->attachComponent<PhysicsComponent>(gGenerateID());
		player->cphysics().setActive(false);
		player->attachComponent<AABBColliderComponent>(player->transform, gGenerateID());

		auto wall = gInst<gEntity>(activeScene);
		wall->transform.position = Vector2(-500, 0);
		wall->transform.size 	 = Vector2(100, 1000);
		wall->attachComponent<AABBColliderComponent>(wall->transform, gGenerateID());
		
		activeScene->addEntity(std::move(floor));
		activeScene->addEntity(std::move(player));
		activeScene->addEntity(std::move(wall));
		gEntity* p_cam = activeScene->addEntity(std::move(camera));
		activeScene->setSkybox(Vector4(1.0f, 0.7f, 0.4f, 1.0f));
		activeScene->setActiveCam(p_cam->id);
		std::cout << "test\n";
		
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
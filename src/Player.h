#pragma once
#include "Entity.h"
#include "Systems/Input.h"
#include "Systems/Time.h"

class gPlayer : public gEntity {
public:
	gPlayer(gScene* s) : gEntity(s) {}
	gPlayer(gScene* s, const std::string &name) : gEntity(s, name) {}

	void tick() override;
private:
	float move = 100.f;
};
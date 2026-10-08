#pragma once
#include "Entity.h"
#include "gRenderer/Camera.h"
#include "gEngineCore.h"

class gCamera : public gEntity {
public:
    gCamera(float left, float right, float bottom, float top, gScene* scene);
    gCamera(gScene* scene);

    void followEntity(gEntity* entity);

    void start() override;
    void tick() override;
    void end() override;

    void realign(Vector4 v4);
    void zoom(float z);
    gRenderer::grCamera &grCamera() const;

protected:
    gEntity* spotlightEntity;
    gRenderer::grCamera* cam;
};
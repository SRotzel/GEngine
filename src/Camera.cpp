#include "Camera.h"

gCamera::gCamera(float left, float right, float bottom, float top, gScene* scene) : gEntity(scene) {
    spotlightEntity = nullptr;
    cam = new gRenderer::grCamera(left, right, bottom, top);
}
gCamera::gCamera(gScene* scene) : gEntity(scene) {
    cam = new gRenderer::grCamera();
}

void gCamera::followEntity(gEntity *entity) {
    spotlightEntity = entity;
    cam->setPosition(Vector3(entity->transform.position, 0.f));
    cam->setRotation(entity->transform.rotation);
    transform = entity->transform;
}

void gCamera::start() {
    gEntity::start();
    if(spotlightEntity) {
        cam->setPosition(Vector3(spotlightEntity->transform.position, 0.f));
        cam->setRotation(spotlightEntity->transform.rotation);
        transform = spotlightEntity->transform;
    }
}

void gCamera::tick() {
    gEntity::tick();
    // Make sure no weird bugs appear....
    //transform.size = Vector2(0,0);

    /*if(!spotlightEntity) return;
    if(transform == spotlightEntity->transform) return;

    transform = spotlightEntity->transform;
    cam->setPosition(gPosToGLPos(Vector3(transform.position, 0.f)));
    //cam->setRotation(transform.rotation);
    */
}

void gCamera::realign(Vector4 v4) {
    if(!cam) return;
    cam->realign(v4.x, v4.y, v4.z, v4.w);
}

gRenderer::grCamera& gCamera::grCamera() const {
    return *cam;
};

void gCamera::end() {
    gEntity::end();
    spotlightEntity = nullptr;
}
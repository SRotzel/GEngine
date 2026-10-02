#pragma once
#include "gEngineCore.h"

#include "Scene.h"
#include "gRenderer/gRenderer2D.h"
#include "Systems/Input.h"
#include "Systems/Time.h"
#include "Components/TextureComponent.h"
#include "Physics.h"

#include <unordered_map>
#include <chrono>

class gEngine;

class gApplication {
public:
    gApplication() {}
    virtual ~gApplication() {}

    virtual void start() {
        gScene* scene = new gScene();
        loadScene(scene); 
    }
    virtual void tick() {
        if(activeScene) activeScene->tick();
    }
    virtual void end() {}

    // called during debug draw calls
    virtual void onDebug() {
        for(const auto& e : activeScene->getEntities()) {
            // Dev Debug gui
            std::string label = "Entity " + std::to_string(e->id);
            if(ImGui::CollapsingHeader(label.c_str())) {
                Transform &t = e->transform;
                ImGui::DragFloat2("position", &t.position.x, 1.f);
                ImGui::DragFloat("rotation", &t.rotation, 1.f);
                ImGui::DragFloat2("size", &t.size.x, 0.1f);
                ImGui::Separator();
                for(auto&& c : e->getComponents()) c->onDebug();
            }
        }
    } 

    void loadScene(gScene*);

protected:
    friend gEngine;
    using Super = gApplication;
    gScene *activeScene = nullptr;
};

class gEngine {
public:
    gEngine(gApplication* = nullptr);
    ~gEngine();

    void addShader(const std::string &path);

    void start();
    void run();
    void end();

    void setCamera(const gCamera&);
    gCamera* getCamera();

private:
    void draw(const gRenderer::VertexArray&, const gRenderer::IndexBuffer&);
    void drawDebugGui() const;
    bool developerDebug = true;

    PhysicsSystem physics;
    gApplication* app = nullptr;
    gRenderer::grCamera      currentCam;

    gRenderer::gRenderer2D   renderer;
    std::unique_ptr<gRenderer::ShaderProgram> program;
    std::vector<std::string> shaderPaths;
};
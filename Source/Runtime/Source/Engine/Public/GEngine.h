#pragma once
#include "GCore.h"

#include "Scene.h"
#include "gRenderer2D.h"
#include "GApplication.h"
#include "Input.h"
#include "Time.h"
#include "TextureComponent.h"
#include "Physics.h"

#include <unordered_map>
#include <chrono>

class gEngine {
public:
    gEngine(GApplication* = nullptr);
    ~gEngine();

    void addShader(const std::string &path);

    void start();
    void run();
    void end();

    void setCamera(const GCamera&);
    GCamera* getCamera();

private:
    void draw(const gRenderer::VertexArray&, const gRenderer::IndexBuffer&);
    void drawDebugGui() const;
    bool developerDebug = true;

    PhysicsSystem physics;
    GApplication* app = nullptr;
    gRenderer::grCamera      currentCam;

    gRenderer::gRenderer2D   renderer;
    std::unique_ptr<gRenderer::ShaderProgram> program;
    std::vector<std::string> shaderPaths;
};
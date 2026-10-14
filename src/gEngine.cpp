#include "gEngine.h"

gEngine::gEngine(gApplication* a) : app(a), developerDebug(true) {
    
}
gEngine::~gEngine() {}

// TODO: refactor
void gApplication::loadScene(gScene *s) {
    if(s == nullptr) {
        std::cout << "Attempting to load invalid Scene!\n";
        return;
    }
    
    if(activeScene) {
        activeScene->end();
        delete activeScene;
        activeScene = s;
        activeScene->start();
        return;
    }

    activeScene = s;
    activeScene->start();
}

gRenderer::ShaderProgram* compileShaderProgram(const std::map<std::string, GLenum> paths) {
    gRenderer::ShaderProgram* prog;
    for(const auto& it : paths) {
        prog->addShader(it.first, it.second);
    }
    prog->link();
    return prog;
}

void gEngine::bindShaderProgram(gRenderer::ShaderProgram* program) {
    this->program = program;
    program->bind();
}

/*void gEngine::setCamera(const gCamera &c) {
    currentCam = c;
}

gCamera *gEngine::getCamera() { return &currentCam; }*/

void gEngine::start() {
    renderer.init(false, "gEngine", 1280, 720);
    renderer.initDebugGUI();
    //currentCam.realign(ortho.x, ortho.y, ortho.z, ortho.w);
    
    program = new gRenderer::ShaderProgram();
    program->addShader("src/shaders/basic.vert", GL_VERTEX_SHADER); 
    program->addShader("src/shaders/basic.frag", GL_FRAGMENT_SHADER); 
    program->link();
    program->bind();
    
    if(!app) return;
    app->start();
    if(app->activeScene) {
        for(auto& e : app->activeScene->getEntities()) {
            e->start();
        }
        Vector4 ortho = Vector4(-(renderer.winInfo.width / 2), renderer.winInfo.width / 2, -(renderer.winInfo.height / 2), renderer.winInfo.height / 2);
        gCamera* activeCam = app->activeScene->getActiveCam();
        if(activeCam != nullptr) activeCam->realign(ortho);
        else std::cout << "Warning: missing active Camera!\n";

        physics.init(app->activeScene);
    }

    developerDebug = true;
    InputManager::init(renderer.window);
}

void gEngine::run() {
    if(!app) return;
    double delta, current;
    double last = 0.0;
    
    gRenderer::BufferLayout  bufferLayout;
    bufferLayout.push<float>(2);
    bufferLayout.push<float>(2);

    gRenderer::VertexArray vao;
    gRenderer::VertexBuffer buffer(gRenderer::grRectangle2D, 4*4*sizeof(float));
    vao.addBuffer(buffer, bufferLayout);

    unsigned int indices[] = {
	    0, 1, 2,
	    2, 3, 0
	};
    gRenderer::IndexBuffer ibo(indices, 6);
    
    gUint framec = 0;
    double fpsTimer = 0;
    while(!glfwWindowShouldClose(renderer.window)) {
        current = glfwGetTime();
        delta = current - last;
        last = current;
        Time::setDelta(delta);

        framec++;
        fpsTimer += delta;
        if(fpsTimer >= 1.0) {
            Time::fps = framec / fpsTimer;
            framec = 0;
            fpsTimer = 0;
        }

        InputManager::tick();
        physics.physicsTick();
        app->tick();
        draw(vao, ibo);
        windowDimensions = {renderer.winInfo.width, renderer.winInfo.height};
    }
}

void gEngine::drawDebugGui() const {
    ImGui::Separator();
    ImGui::Value("FPS: ", Time::fps);

    if(ImGui::Button("Create new Entity")) {
        app->activeScene->addEntity(std::make_unique<gEntity>(app->activeScene));
    }

    ImGui::Separator();
}

void gEngine::draw(const gRenderer::VertexArray& vao, const gRenderer::IndexBuffer &ibo) {
    renderer.clear(app->activeScene->getSkybox());
    if(developerDebug) {
        renderer.beginSceneDebug(app->activeScene->getActiveCam()->grCamera(), *program);
        if(ImGui::Checkbox("fullscreen", &renderer.winInfo.fullscreen)) 
            renderer.setFullscreen(renderer.winInfo.fullscreen, &app->activeScene->getActiveCam()->grCamera()); // TODO: maybe refactor (is pointer necessary?)
        drawDebugGui();
        app->onDebug(); 
    } else renderer.beginScene(app->activeScene->getActiveCam()->grCamera(), *program);

    if(app->activeScene) {
        for(const auto &e : app->activeScene->getEntities()) {
            //Matrix4 model = getModelMatrix(e->transform);
            
            program->bind();
            program->setUniformMat4f("u_mvp", e->transform.getMatrix());

            if(const auto &c = e->getComponent<TextureComponent>()) {
                c->getTexture()->bind(0);
                program->setUniform1i("u_texture", 0);
                renderer.submit(vao, ibo, *program);
                c->getTexture()->unbind();
                continue;
            }
            
            renderer.submit(vao, ibo, *program);
        }
    }
    if(developerDebug) renderer.submitGUIData();
    renderer.endScene();
}

void gEngine::end() {
    if(app) app->end();
    InputManager::end();
    renderer.free();
}
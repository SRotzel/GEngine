#include "gRenderer2D.h"

namespace gRenderer {
    void GLAPIENTRY glErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
        // ignore non-significant error/warning codes
        if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

        std::cout << "Debug: (" << message << ") " << "id:" << id << std::endl;
        switch (source)
        {
            case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
            case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
            case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
        } std::cout << std::endl;
    
        switch (type)
        {
            case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
            case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
            case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
            case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
            case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
            case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
            case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
        } std::cout << std::endl;
        
        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
            case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
            case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
            case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
        } std::cout << std::endl;
        std::cout << std::endl;
    }

    void framebuffer_size_callback(GLFWwindow* win, int width, int height) {
        glViewport(0, 0, width, height);
    }

    gRenderer2D::gRenderer2D()  {}
    gRenderer2D::~gRenderer2D() {}

    void gRenderer2D::init(bool fullscreen, const char* title, int x, int y) {
        if(!glfwInit()) {
            std::cerr << "Failed to initialize gRenderer2D!" << std::endl;
            return;
        }
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

        winInfo.title = title;
        winInfo.posX = 100;
        winInfo.posY = 100;
        winInfo.width = x;
        winInfo.height = y;
        winInfo.fullscreen = fullscreen;
        
        window = glfwCreateWindow(x, y, title, nullptr, nullptr); // fullscreen ? monitor : nullptr
        if(!window) {
            throw std::runtime_error("Failed to create GLFW Window!");
        }
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        setFullscreen(fullscreen, nullptr);

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); // TODO: Fix this (fps) -> dont work on linux bwoah
        gladLoadGL();
        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glDebugMessageCallback(glErrorCallback, 0);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void gRenderer2D::initDebugGUI() {
        debugGUI = true;
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        ImGui::StyleColorsDark();

        float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
        ImGuiStyle &style = ImGui::GetStyle();
        style.ScaleAllSizes(main_scale);
        style.FontScaleDpi = main_scale;
        style.WindowRounding = 7.f;

        ImGui_ImplGlfw_InitForOpenGL(this->window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        ImGui::GetIO().Fonts->AddFontDefault();
        ImFont* robotoMonoFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("src/gRenderer/RobotoMono.ttf", 13.f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesDefault());
        IM_ASSERT(robotoMonoFont != NULL);
        ImGui::PushFont(robotoMonoFont);

        debugGUI = true;
    }

    void gRenderer2D::beginScene(const grCamera &cam, ShaderProgram &program) {
        /*int x, y;
        glfwGetFramebufferSize(window, &x, &y);
        std::cout << x << ":" << y << std::endl;
        cam.realign(0.0f, x, y, 0.0f);*/

        glfwPollEvents();
        program.bind();
        program.setUniformMat4f("u_viewProj", cam.getViewProj());
    }

    void gRenderer2D::beginSceneDebug(const grCamera &cam, ShaderProgram &program) {
        glfwPollEvents();
        this->beginScene(cam, program);
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("gEngine Debug window");
    }

    void gRenderer2D::submitGUIData() {
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void gRenderer2D::submit(const VertexArray& va, const IndexBuffer& ib, const ShaderProgram& prog) const {
        if(G_LIMIT_MINIMIZED_PERFORMANCE && glfwGetWindowAttrib(window, GLFW_ICONIFIED)) return;
        prog.bind();
        va.bind();
        ib.bind();
        glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
    }

    void gRenderer2D::clear(glm::vec4 color) const {
        glClearColor(color.x, color.y, color.z, color.w);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void gRenderer2D::endScene() {
        glfwSwapBuffers(window);
    }

    void gRenderer2D::setFullscreen(bool fullscreen, grCamera* cam) {
        if(fullscreen) {
            glfwGetWindowPos(window, &winInfo.posX, &winInfo.posY);
            glfwGetWindowSize(window, &winInfo.width, &winInfo.height);

            GLFWmonitor *moni = glfwGetPrimaryMonitor();
            const GLFWvidmode *mode = glfwGetVideoMode(moni);
            glfwSetWindowMonitor(window, moni, 0, 0, mode->width, mode->height, mode->refreshRate);

            if(!cam) return;
            cam->realign(-(mode->width) / 2, mode->width / 2, -(mode->height) / 2, mode->height / 2);
        } else {
            glfwSetWindowMonitor(window, nullptr, winInfo.posX, winInfo.posY, winInfo.width, winInfo.height, 0);

            if(!cam) return;
            cam->realign(-(winInfo.width) / 2, winInfo.width / 2, -(winInfo.height) / 2, winInfo.height / 2);
        }
    }

    void gRenderer2D::free() {
        if(debugGUI) {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }
        glfwDestroyWindow(this->window);
        glfwTerminate();
    }
}
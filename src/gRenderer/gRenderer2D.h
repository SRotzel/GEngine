#pragma once
#include "../../include/glad/glad.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shaders.h"
#include "Texture.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include "../../include/imgui/imgui.h"
#include "../../include/imgui/backends/imgui_impl_glfw.h"
#include "../../include/imgui/backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
//#include <imgui.h>
//#include <imgui_impl_glfw.h>
//#include <imgui_impl_opengl3.h>

// Ignore draw calls when the window is minimized
#define G_LIMIT_MINIMIZED_PERFORMANCE true

namespace gRenderer {
    void GLAPIENTRY glErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
    void framebuffer_size_callback(GLFWwindow* win, int width, int height);

    constexpr float grRectangle2D[16] = {
    	-0.5f,-0.5f, 0.f, 0.f,
    	 0.5f,-0.5f, 1.f, 0.f,
    	 0.5f, 0.5f,1.f,1.f,
       -0.5f,0.5f,0.f,1.f
    };

    typedef struct {
        const char* title;
        int width, height;
        int posX, posY;
        bool fullscreen;
    } gWinInfo;

    class gRenderer2D {
        public:
            gRenderer2D();
            ~gRenderer2D();

            void init(bool fullscreen = false, const char* title = "gRenderer application", int x = 800, int y = 600);
            void initDebugGUI();
            void addDebugGUIItem();

            void beginScene(const grCamera &cam, ShaderProgram &program);
            void beginSceneDebug(const grCamera &cam, ShaderProgram &program);

            
            void submit(const VertexArray&, const IndexBuffer&, const ShaderProgram&) const;
            void submitGUIData();

            void endScene();
            void clear(glm::vec4 color) const;

            void free();

            void setFullscreen(bool fullscreen, grCamera *cam);
            inline bool quit() const { return glfwWindowShouldClose(window); }

            GLFWwindow* window;
            gWinInfo winInfo;
        private:
            GLuint vertexShader, fragmentShader;
            GLFWmonitor* monitor;

            unsigned int loadShader(const std::string& path, unsigned int type);
            unsigned int compileShaders();

            bool debugGUI;
    };
}
#pragma once

#include "GCore.h"
#include "Forward.h"

#include <GLFW/glfw3.h>

class InputManager {
public:
    static void init(GLFWwindow* window);

    
    static bool isKeyPressed(int key);
    static bool isKeyReleased(int key);
    static bool isKeyHeld(int key);
    static Vector2 getMousePos();
    
    
private:
    friend GEngine;
    static void tick();
    static void end();
    
    static GLFWwindow* context;
    static bool previousKeySnapshot[GLFW_KEY_LAST + 1];
    static bool currentKeySnapshot[GLFW_KEY_LAST + 1];

    static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos); 
    static double mouseX, mouseY;
};
#include "Input.h"
#include <GLFW/glfw3.h>

static void i_cursor_pos_callback(GLFWwindow *window, double xpos, double ypos) {

}

GLFWwindow* InputManager::context = nullptr;
bool InputManager::previousKeySnapshot[GLFW_KEY_LAST + 1]{};
bool InputManager::currentKeySnapshot[GLFW_KEY_LAST+1]{};
double InputManager::mouseX = 0.0;
double InputManager::mouseY = 0.0;

void InputManager::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    mouseX = xpos;
    mouseY = ypos;
}

void InputManager::init(GLFWwindow* window) {
    context = window;
    //glfwSetCursorPosCallback(context, cursor_pos_callback);
}

// TODO maybe only use one for loop
void InputManager::tick() {
    for(int key = 0; key <= GLFW_KEY_LAST; ++key) {
        previousKeySnapshot[key] = currentKeySnapshot[key];
    }
        
    for(int key = 0; key <= GLFW_KEY_LAST; ++key) {
        currentKeySnapshot[key] = glfwGetKey(context, key) == GLFW_PRESS;
    }

    glfwPollEvents();
}

bool InputManager::isKeyHeld(int key) {
    if(key < 0 || key > GLFW_KEY_LAST) return false;
    return currentKeySnapshot[key];
}

bool InputManager::isKeyPressed(int key) {
    if(key < 0 || key > GLFW_KEY_LAST) return false;
    return currentKeySnapshot[key] && !previousKeySnapshot[key];
}

bool InputManager::isKeyReleased(int key) {
    if(key < 0 || key > GLFW_KEY_LAST) return false;
    return !currentKeySnapshot[key] && previousKeySnapshot[key];
}

Vector2 InputManager::getMousePos() {
    return Vector2(mouseX, mouseY);
}

void InputManager::end() {
    
}
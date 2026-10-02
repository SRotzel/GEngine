// basic engine utilities and types
// mustn't include another gEngine file 

#pragma once

#include "gRenderer/Camera.h"
#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <iostream>

using gUint = unsigned int;
static gUint gidIndex = 1;
static gUint gGenerateID() { 
    return gidIndex++; 
}

using Vector2 = glm::vec2;
using Vector3 = glm::vec3;
using Vector4 = glm::vec4;
using Matrix4 = glm::mat4;

static inline Vector2 v2zero() { return Vector2 {0.f, 0.f}; }
static inline Vector3 v3zero() { return Vector3 {0.f, 0.f, 0.f}; }
static inline Vector4 v4zero() { return Vector4 {0.f, 0.f, 0.f, 0.f}; }

using gCamera = gRenderer::grCamera;

struct Transform {
    glm::vec2 position{0.f, 0.f};
    glm::vec2 size{100.f, 100.f};
    float rotation {0.f};

    glm::mat4 getMatrix() const {
        glm::mat4 m = glm::translate(glm::mat4(1.f), glm::vec3(position, 0.f)); // ignore z
        m = glm::rotate(m, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        m = glm::scale(m, glm::vec3(size, 1.0f));
        return m;
    }

    void translate(const glm::vec2& delta) { position += delta; }
    void rotate(float rad)                 { rotation += rad; }

    glm::vec2 forward() const { return glm::vec2(std::cos(rotation), std::sin(rotation)); }
    glm::vec2 right() const   { return glm::vec2(-std::sin(rotation), std::cos(rotation)); }
};

static glm::mat4 getModelMatrix(const Transform& t) {
    return glm::translate(Matrix4(1.0f), glm::vec3(t.position, 0.0f)) 
                        * glm::rotate(glm::mat4(1.0f), -glm::radians(t.rotation), glm::vec3(0.f, 0.f, 1.f))
                        * glm::scale(glm::mat4(1.0f), glm::vec3(t.size, 0.f) 
            );
}
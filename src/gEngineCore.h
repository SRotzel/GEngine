// basic engine utilities and types
// mustn't include another gEngine file 

#pragma once

#include <memory>
#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <iostream>

// validate unique identifiers. Returns true on valid ids
#define gValidateID(x) x != 0; 

#ifndef SCALE 
#define SCALE 10.0f
#endif

using gUint = unsigned int;
static gUint gidIndex = 1;
static gUint gGenerateID() { 
    return gidIndex++; 
}

static glm::vec2 windowDimensions;

static void debug(std::string message) {
    
}

using Vector2 = glm::vec2;
using Vector3 = glm::vec3;
using Vector4 = glm::vec4;
using Matrix4 = glm::mat4;

using std::move, std::unique_ptr, std::forward;
template<typename T, typename... Args>
unique_ptr<T> gInst(Args&&... a) {
    return std::make_unique<T>(std::forward<Args>(a)...);
}

static inline Vector2 v2zero() { return Vector2 {0.f, 0.f}; }
static inline Vector3 v3zero() { return Vector3 {0.f, 0.f, 0.f}; }
static inline Vector4 v4zero() { return Vector4 {0.f, 0.f, 0.f, 0.f}; }

struct Transform {
    bool operator==(const Transform& other) const {
        if(position == other.position && size == other.size && rotation == other.rotation) return true;
        return false;
    }

    glm::vec2 position{0.f, 0.f};
    glm::vec2 size{100.f, 100.f};
    float rotation {0.f};

    glm::mat4 getMatrix() const {
         return glm::translate(Matrix4(1.0f), glm::vec3(position, 0.0f))
         * glm::rotate(glm::mat4(1.0f), -glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f))
         * glm::scale(glm::mat4(1.0f), glm::vec3(size, 0.f));
    }

    void translate(const glm::vec2& delta) { position += delta; }
    void rotate(float rad)                 { rotation += rad; }

    glm::vec2 forward() const { return glm::vec2(std::cos(rotation), std::sin(rotation)); }
    glm::vec2 right() const   { return glm::vec2(-std::sin(rotation), std::cos(rotation)); }
};

// converts a position in gEngine coordinates to openGL coordinates
static glm::vec3 gPosToGLPos(glm::vec3 gpos) {
    glm::vec3 glPos = {gpos.x / windowDimensions.x, gpos.y / windowDimensions.y, 0.f};
    return glPos;
}
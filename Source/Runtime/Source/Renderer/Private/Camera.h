#pragma once
#include <glm/gtc/matrix_transform.hpp>

namespace gRenderer {
    class grCamera {
    public:
        grCamera(float left, float right, float bottom, float top) 
            : camRotation(0.0f), camPosition(0.0f), projection(glm::ortho(left, right, bottom, top)) 
        {
            recalcViewMatrix();
        }
        ~grCamera() {}

        void setPosition(const glm::vec3& pos )     { camPosition = pos;     recalcViewMatrix(); }
        void move(const glm::vec3& delta)           { camPosition += delta;  recalcViewMatrix(); }
        void setRotation(float degrees)             { camRotation = degrees; recalcViewMatrix(); }
        void rotate(float delta)                    { camRotation += delta;  recalcViewMatrix(); }

        void realign(float left, float right, float bottom, float top) {
            projection = glm::ortho(left, right, bottom, top);
            recalcViewMatrix();
        }

        inline glm::vec3 getPosition() const { return camPosition; }
        inline glm::mat4 getViewProj() const { return viewProjection; }
        
    private:
        void recalcViewMatrix() {
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), camPosition) 
                                * glm::rotate(glm::mat4(1.0f), glm::radians(camRotation), glm::vec3(0, 0, 1));
            view = transform;
            viewProjection = view * projection;
        }

        float camRotation;
        glm::vec3 camPosition;
        glm::mat4 projection;
        glm::mat4 view;
        glm::mat4 viewProjection;
    };
}
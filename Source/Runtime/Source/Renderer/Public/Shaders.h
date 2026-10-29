#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

#include "glad/glad.h"

namespace gRenderer {
    class ShaderProgram {
    public:
        // Initiates a shader. path = file path to shader source, type = type of shader (Vertex, fragment...)
        ShaderProgram();
        ~ShaderProgram();

        // add and compile shader: 0⁼success, -1⁼failure
        int addShader(const std::string& path, GLenum type);
        void link();
        void bind() const;
        void unbind() const;

        void setUniform1i(const std::string &name, int value);
        void setUniform1f(const std::string &name, float value);
        void setUniform4f(const std::string &name, float v0, float v1, float v2, float v3);
        void setUniformMat4f(const std::string &name, const glm::mat4& mat);

        void deleteProgram() const;

        inline const unsigned int getRenderID() { return renderID; }

    private:
        unsigned int renderID;
        // caching for uniforms (?)
        std::vector<unsigned int> shaders;
        std::unordered_map<std::string, int> uniformLocationCache;

        int uniformloc(const std::string &name);
    };
}
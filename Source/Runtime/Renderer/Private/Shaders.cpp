#include "Shaders.h"

namespace gRenderer {
    ShaderProgram::ShaderProgram() { 
        renderID = glCreateProgram();
    } 
    ShaderProgram::~ShaderProgram() {
        glDeleteProgram(renderID);
    }

    void ShaderProgram::link() {
        std::cout << "Linking Shader Program...\n";
        glLinkProgram(renderID);
        for(unsigned int i : shaders) {
            std::cout << "deleting shader... ID: [" << i << "]\n";
            glDetachShader(renderID, i);
            glDeleteShader(i);
        }
    }

    void ShaderProgram::bind() const    { glUseProgram(renderID); }
    void ShaderProgram::unbind() const  { glUseProgram(0); }

    int ShaderProgram::addShader(const std::string &path, GLenum type) {
        std::cout << "Compiling shader \"" << path << "\"\n";

        // read shader source code from file
        std::ifstream stream(path);
        if(!stream.is_open())
            throw std::runtime_error("Failed to open shader: " + path);

        std::stringstream buffer;
        buffer << stream.rdbuf();
        std::string ssrc = buffer.str();
        const char* src  = ssrc.c_str();

        // Create and compile shader 
        unsigned int shaderID = glCreateShader(type);
        glShaderSource(shaderID, 1, &src, nullptr);
        glCompileShader(shaderID);

        // check for compilation errors
        int success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success); 
        if(!success) {
            int l;
            glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &l);
            char log[l];
            glGetShaderInfoLog(shaderID, l, nullptr, log);
            std::cerr << "Failed to compile shader: " << path << log << std::endl;
            return -1;
        }

        std::cout << "success! ID: [" << shaderID << "]\n";
        shaders.push_back(shaderID);
        glAttachShader(renderID, shaderID);
        return 0;
    }

    void ShaderProgram::deleteProgram() const {
        glDeleteProgram(renderID);
    }

    int ShaderProgram::uniformloc(const std::string &name) {
        if(uniformLocationCache.find(name) != uniformLocationCache.end()) return uniformLocationCache[name];

        int location = glGetUniformLocation(renderID, name.c_str());
        if (location == -1) 
            std::cout << "Warning: Uniform " << name << " doesnt exist\n";
        else 
            uniformLocationCache[name] = location;

        return location;
    }

    void ShaderProgram::setUniform4f(const std::string &name, float v0, float v1, float v2, float v3) {
        glUniform4f(uniformloc(name), v0, v1, v2, v3);
    }
    void ShaderProgram::setUniform1f(const std::string &name, float value) {
        glUniform1f(uniformloc(name), value);
    }
    void ShaderProgram::setUniform1i(const std::string &name, int value) {
        glUniform1i(uniformloc(name), value);
    }
    void ShaderProgram::setUniformMat4f(const std::string &name, const glm::mat4 &mat) {
        glUniformMatrix4fv(uniformloc(name), 1, GL_FALSE, glm::value_ptr(mat));
    }
}
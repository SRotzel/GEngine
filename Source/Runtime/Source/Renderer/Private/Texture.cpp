#include "Texture.h"

namespace gRenderer {
    Texture::Texture(const std::string &path) : renderID(0), filePath(path), localBuffer(nullptr), width(0), height(0), bpp(0) {
        std::cout << "Loading Texture " << path << std::endl;
        stbi_set_flip_vertically_on_load(1); // flip texture because opengl reads pixels from bottom left
        localBuffer = stbi_load(path.c_str(), &width, &height, &bpp, 4); // 4 -> rgba = 4 channels (1: r, 2: g...)
        
        glGenTextures(1, &renderID);
        glBindTexture(GL_TEXTURE_2D, renderID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // s|t ~ x|y for textures

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
        glBindTexture(GL_TEXTURE_2D, 0);

        if(localBuffer) stbi_image_free(localBuffer); // MAYBE keep around for sampling etc
    }

    Texture::~Texture() {
        glDeleteTextures(1, &renderID);
    }

    void Texture::bind(unsigned int slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);  
        glBindTexture(GL_TEXTURE_2D, renderID);
    }
    
    void Texture::unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
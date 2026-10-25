#pragma once
#include <string>
#include <iostream>
#include "../../include/glad/glad.h"
#include "../../include/stb/stb_image.h"

namespace gRenderer {
    class Texture {
    public:
        Texture(const std::string &path);
        ~Texture();

        void bind(unsigned int slot) const; // alternatively slot = 0;
        void unbind() const;

    private: 
        unsigned int renderID;
        std::string filePath;
        unsigned char* localBuffer;
        int width, height, bpp; // bits per pixel
    };
}
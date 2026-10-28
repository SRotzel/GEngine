#pragma once
#include "glad/glad.h"

namespace gRenderer {
    class IndexBuffer {
    public:
        IndexBuffer(const unsigned int* data, unsigned int count);
        ~IndexBuffer();

        void bind()   const;
        void unbind() const;
        unsigned int getCount() const;
    private:
        unsigned int renderID;
        unsigned int count;
    };
}
#pragma once
#include "VertexBuffer.h"
#include "BufferLayout.h"
#include <vector>
#include <iostream>

namespace gRenderer {
    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        void bind() const;
        void unbind() const;
        void addBuffer(const VertexBuffer& buffer, const BufferLayout& bl);

    private:
        unsigned int renderID;
    };
}
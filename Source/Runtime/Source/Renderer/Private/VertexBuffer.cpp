#include "VertexBuffer.h"

namespace gRenderer {
    VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
        glGenBuffers(1, &renderID);
        glBindBuffer(GL_ARRAY_BUFFER, renderID);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); 
    }
    VertexBuffer::~VertexBuffer() { 
        glDeleteBuffers(1, &renderID);
    }

    void VertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, renderID);
    }

    void VertexBuffer::unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
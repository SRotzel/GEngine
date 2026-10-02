#include "IndexBuffer.h"
#include <assert.h>

namespace gRenderer {
    IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) 
        : count(count) {
        assert(sizeof(unsigned int) == sizeof(GLuint));

        glGenBuffers(1, &renderID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW); // Dangerous -> GLuint might not be unsigned int in size
    }
    IndexBuffer::~IndexBuffer() { 
        glDeleteBuffers(1, &renderID);
    }

    void IndexBuffer::bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderID);
    }

    void IndexBuffer::unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    unsigned int IndexBuffer::getCount() const { return this->count; }
}
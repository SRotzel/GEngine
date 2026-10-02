#include "VertexArray.h"

namespace gRenderer {
    VertexArray::VertexArray() {
        glGenVertexArrays(1, &renderID);
    }
    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &renderID);
    }

    void VertexArray::bind()   const  { glBindVertexArray(renderID); }
    void VertexArray::unbind() const  { glBindVertexArray(0); }

    void VertexArray::addBuffer(const VertexBuffer &buffer, const BufferLayout &bl) {
        this->bind();
        buffer.bind();
        const auto& elements = bl.getElements();
        unsigned int offset = 0;

        for (unsigned int i = 0; i < elements.size(); i++) {
            const auto& e = elements[i];
            glEnableVertexAttribArray(i);   
            void *ptr = reinterpret_cast<void*>(static_cast<uintptr_t>(offset));
            glVertexAttribPointer(i, e.count, e.type, e.normalized, bl.getStride(), ptr);
            offset += e.count * GLsizeof(e.type);
        }
    }
};
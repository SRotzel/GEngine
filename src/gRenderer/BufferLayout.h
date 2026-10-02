#include "../../include/glad/glad.h"
#include <vector>
#include <iostream>

namespace gRenderer {
    // turns an enum (e.g GL_FLOAT) into the correct byte size
    static inline unsigned int GLsizeof(GLenum type) {
        switch(type) {
            case GL_FLOAT:          return 4;
            case GL_UNSIGNED_INT:   return 4;
            case GL_UNSIGNED_BYTE:  return 1;
        }
        std::cout << "Used unsupported GLenum in GLsizeof!\n";
        return 0;   
    }

    struct BufferElement {
        unsigned int count;
        unsigned int type;
        unsigned char normalized;
    };

    class BufferLayout {
    public:
        BufferLayout() : stride(0) { }
        ~BufferLayout() {}

        template<typename T>
        inline void push(unsigned int count) {
            std::cout << "fuck\n";
        }

        inline std::vector<BufferElement> getElements() const { return elements; }
        inline unsigned int getStride() const { return stride; }

    private: 
        std::vector<BufferElement> elements;
        unsigned int stride;
    };

    template<> inline void BufferLayout::push<float>(unsigned int count) {
        std::cout << "all good\n";
        elements.push_back({count, GL_FLOAT, GL_FALSE });
        stride += count * GLsizeof(GL_FLOAT);
    }
    template<> inline void BufferLayout::push<unsigned int>(unsigned int count) {
        elements.push_back({count, GL_UNSIGNED_INT, GL_FALSE});
        stride += count * GLsizeof(GL_UNSIGNED_INT);
    }
    template<> inline void BufferLayout::push<unsigned char>(unsigned int count) {
        elements.push_back({count, GL_UNSIGNED_BYTE, GL_TRUE});
        stride += count * GLsizeof(GL_UNSIGNED_BYTE);
    }
}
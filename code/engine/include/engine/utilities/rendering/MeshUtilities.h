#include "OpenGL/OpenGlInclude.h"

template<typename T>
GLuint CreateBuffer(GLuint bufferType, const std::vector<T>& bufferData) {
    unsigned int buffer = 0;
    glGenBuffers(1, &buffer);
    glBindBuffer(bufferType, buffer);
    glBufferData(bufferType,
        bufferData.size() * sizeof(glm::vec3),
        bufferData.data(),
        GL_STATIC_DRAW);
    return buffer;
}
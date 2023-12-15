#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(GLuint *indices, GLsizei count) : Count(count) {
    glGenBuffers(1, &(this->IndexBufferID));
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * count, indices, GL_STATIC_DRAW);

}

IndexBuffer::~IndexBuffer() {

    glDeleteBuffers(1, &IndexBufferID);
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);
}

void IndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
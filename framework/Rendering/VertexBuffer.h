#ifndef VERTEXBUFFER_H_
#define VERTEXBUFFER_H_

#include <glad/glad.h>
#include "BufferLayout.h"

class VertexBuffer
{
public:
    // Constructor: initializes the VertexBuffer with a data buffer and its size.
    // Note that the buffer is bound upon construction.
    VertexBuffer(const void *vertices, GLsizei size);
    ~VertexBuffer();

    // Bind the VertexBuffer
    void Bind() const;

    // Unbind the VertexBuffer
    void Unbind() const;

    // Fill a specific segment of the buffer specified by an offset and size with data.
    void BufferSubData(GLintptr offset, GLsizeiptr size, const void *data) const;

    // Set/Get buffer layout
    const BufferLayout& GetLayout() const { return Layout; }
    void SetLayout(const BufferLayout& layout) { Layout = layout; }


private:
    GLuint VertexBufferID;
    static int count;
    BufferLayout Layout;
};

#endif // VERTEXBUFFER_H_
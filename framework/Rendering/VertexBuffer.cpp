#include <iostream>
#include "VertexBuffer.h"

int VertexBuffer::count = 0;

VertexBuffer::VertexBuffer(const void *vertices, GLsizei size)
{
    glGenBuffers(1, &VertexBufferID);
    Bind();
    // transfer data to GPU
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    count++;
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &VertexBufferID);
    count--;
    // cout << "new number of triangles is: " << triangle_counter << endl;
}

void VertexBuffer::Bind() const
{

    // make the vertex buffer active
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::BufferSubData(GLintptr offset, GLsizeiptr size, const void* data) const
{
    // Update a specific segment of the buffer with new data
    // replacement starts at "offset"-bytes into the array
    // replacement goes "size"-bytes far
    // this part will be replaced by data
    Bind();
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

//void VertexBuffer::SetLayout(const BufferLayout& layout)
//{
//}

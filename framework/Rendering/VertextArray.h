#ifndef PROG2002_VERTEXTARRAY_H
#define PROG2002_VERTEXTARRAY_H


#include <memory>
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class VertexArray {
public:
    // Constructor & Destructor
    VertexArray();
    ~VertexArray();

    // Bind vertex array
    void Bind() const;
    // Unbind vertex array
    void Unbind() const;

    // Add vertex buffer. This method utilizes the BufferLayout internal to
    // the vertex buffer to set up the vertex attributes. Notice that
    // this function opens for the definition of several vertex buffers.
    void AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer);
    // Set index buffer
    void SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer);

    // Get the index buffer
    const std::shared_ptr<IndexBuffer> &GetIndexBuffer() const { return IdxBuffer; }

    const std::shared_ptr<VertexBuffer> &getVertexBuffer(unsigned int idx) const { return VertexBuffers[idx]; }

    const unsigned int getNumberOfVertexBuffers() const { return VertexBuffers.size(); }

private:
    GLuint m_vertexArrayID = 0;
    std::vector<std::shared_ptr<VertexBuffer>> VertexBuffers;
    std::shared_ptr<IndexBuffer> IdxBuffer;

    // Get the vertex buffers
    const std::vector<std::shared_ptr<VertexBuffer>> &GetVertexBuffers() const { return VertexBuffers; }
};


#endif //PROG2002_VERTEXTARRAY_H

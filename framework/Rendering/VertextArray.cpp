#include <iostream>
#include "VertextArray.h"

void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) {
    //Iterate over Layout.begin()
    Bind();

    vertexBuffer->Bind();

    unsigned countBufferLayout = 0;
    auto Layout = vertexBuffer->GetLayout();
    for (auto i = Layout.begin(); i != Layout.end(); ++i) {
        // get the attribute
        auto attribute = *i;
        glVertexAttribPointer(countBufferLayout, ShaderDataTypeComponentCount(attribute.Type),
                              ShaderDataTypeToOpenGLBaseType(attribute.Type), attribute.Normalized,
                              Layout.GetStride(), (const void *) i->Offset);
        glEnableVertexAttribArray(countBufferLayout);

        countBufferLayout++;
    }
    VertexBuffers.push_back(vertexBuffer);
}

void VertexArray::Bind() const {
    glBindVertexArray(m_vertexArrayID);

}

VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_vertexArrayID);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &m_vertexArrayID);
}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
    IdxBuffer->Unbind();

    for (auto &vertexBuffer : VertexBuffers) {
        vertexBuffer->Unbind();
    }
}

void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) {
    IdxBuffer = indexBuffer;
    indexBuffer->Bind();
}

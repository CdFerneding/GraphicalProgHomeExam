#ifndef PROG2002_RENDERCOMMANDS_H
#define PROG2002_RENDERCOMMANDS_H

#include <memory>
#include "glad/glad.h"
#include "VertextArray.h"

namespace RenderCommands
{
    inline void Clear(GLuint mode = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    {
        glClear(mode);
    }

    inline void SetPolygonMode(GLenum face, GLenum mode)
    {
        glPolygonMode(face, mode);
    }

    inline void DrawIndex(GLenum primitive, const std::shared_ptr<VertexArray>& vao)
    {
        vao->Bind();
        glDrawElements(primitive, vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }
    inline void SetClearColor(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
    }
    inline void SetWireframeMode()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    inline void SetSolidMode()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}


#endif //PROG2002_RENDERCOMMANDS_H

#ifndef PROG2002_BUFFERATTRIBUTE_H
#define PROG2002_BUFFERATTRIBUTE_H

#include <string>
#include "ShaderDataTypes.h"

struct BufferAttribute {
    // Constructor
    BufferAttribute(ShaderDataType type, const std::string &name, GLboolean normalized = false)
            : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0),
              Normalized(normalized) {}

    ~BufferAttribute() = default;

    std::string Name;
    ShaderDataType Type;
    GLuint Size;
    GLuint Offset;
    GLboolean Normalized;
};


#endif //PROG2002_BUFFERATTRIBUTE_H

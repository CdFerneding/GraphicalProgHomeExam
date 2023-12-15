#include "BufferLayout.h"

void BufferLayout::CalculateOffsetAndStride() {
    GLsizei offset = 0;
    this->Stride = 0;
    for (auto &attribute : Attributes) {
        attribute.Offset = offset;
        offset += attribute.Size;
        this->Stride += attribute.Size;
    }
}
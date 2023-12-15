#ifndef PROG2002_BUFFERLAYOUT_H
#define PROG2002_BUFFERLAYOUT_H


#include <initializer_list>
#include <vector>
#include "BufferAttribute.h"

class BufferLayout {
    public:
        BufferLayout() {}
        BufferLayout(const std::initializer_list<BufferAttribute> &attributes)
                : Attributes(attributes), Stride(0) {
            this->CalculateOffsetAndStride();
        }

        inline const std::vector<BufferAttribute> &GetAttributes() const { return this->Attributes; }
        inline GLsizei GetStride() const { return this->Stride; }

        std::vector<BufferAttribute>::iterator begin() { return this->Attributes.begin(); }
        std::vector<BufferAttribute>::iterator end() { return this->Attributes.end(); }
        std::vector<BufferAttribute>::const_iterator begin() const { return this->Attributes.begin(); }
        std::vector<BufferAttribute>::const_iterator end() const { return this->Attributes.end(); }

    private:
        void CalculateOffsetAndStride();

    private:
        std::vector<BufferAttribute> Attributes;
        GLsizei Stride;
};



#endif //PROG2002_BUFFERLAYOUT_H

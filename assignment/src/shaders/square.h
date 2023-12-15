#include <string>
#ifndef PROG2002_SQUARE_H
#define PROG2002_SQUARE_H

// Vertex and fragment shader source code
const std::string VS_Square = R"(
    #version 430 core
    layout(location = 0) in vec3 position;

    uniform mat4 u_Model;
    uniform mat4 u_View;
    uniform mat4 u_Projection;

    void main()
    {
        gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0);
    }
)";

const std::string FS_Square = R"(
    #version 430 core

    uniform vec3 u_Color;
    uniform float u_Opacity;

    out vec4 color;
    
    void main()
    {
        color = vec4(u_Color.rgb, u_Opacity);
    }
)";



#endif //PROG2002_SQUARE_H

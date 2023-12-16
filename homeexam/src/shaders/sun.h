#include <string>
#ifndef HOMEEXAM_SUN_H
#define HOMEEXAM_SUN_H

// Vertex and fragment shader source code
const std::string VS_Sun = R"(
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

const std::string FS_Sun = R"(
    #version 430 core

    out vec4 color;
    
    void main()
    {
        color = vec4(1.0);
    }
)";

#endif //HOMEEXAM_SUN_H

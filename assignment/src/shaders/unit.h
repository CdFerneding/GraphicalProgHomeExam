#include <string>
#ifndef PROG2002_UNIT_H
#define PROG2002_UNIT_H

// Vertex and fragment shader source code
const std::string VS_Unit = R"(
    #version 430 core
    layout(location = 0) in vec3 position;

    out vec3 TexCoords;

    uniform mat4 u_Model;
    uniform mat4 u_View;
    uniform mat4 u_Projection;

    void main()
    {;
        TexCoords = position;
        gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0);
    }
)";

const std::string FS_Unit = R"(
    #version 430 core

    in vec3 TexCoords;
    
    uniform samplerCube CubeMap;
    uniform vec3 u_Color;
    uniform float u_Opacity;
    uniform float u_TextureState;

    out vec4 color;
    
    void main()
    {
        if(u_TextureState != 0.0f) {
             //Sample the texture using the texture coordinates
             vec4 texColor = texture(CubeMap, TexCoords);
             color = mix(vec4(texColor.rgb, u_Opacity), vec4(u_Color.rgb, 1.0), 0.5);
        } else {
            color = vec4(u_Color.rgb, u_Opacity);
        }
    }
)";

#endif //PROG2002_UNIT_H

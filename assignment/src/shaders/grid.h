#include <string>
#ifndef PROG2002_GRID_H
#define PROG2002_GRID_H

// Vertex and fragment shader source code
const std::string VS_Grid = R"(
    #version 430 core
    layout(location = 0) in vec3 position;
    layout(location = 1) in vec4 color;
    layout(location = 2) in vec2 texCoords;

    out vec2 fragTexCoords;
    out vec4 fragColor;

    uniform mat4 u_Model;
    uniform mat4 u_View;
    uniform mat4 u_Projection;

    void main()
    {
        gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0);
        fragTexCoords = texCoords;
        fragColor = color;
    }
)";

const std::string FS_Grid = R"(
    #version 430 core

    in vec2 fragTexCoords;
    in vec4 fragColor;

    uniform sampler2D u_Texture;
    uniform float u_TextureState;

    out vec4 color;
    
    void main()
    {
        if(u_TextureState != 0.0f) {
            // Sample the texture using the texture coordinates
            vec4 texColor = texture(u_Texture, fragTexCoords);

            // Mix the color from the vertex attribute with the texture color
            // Adjust the alpha value manually
            vec4 mixedColor = mix(fragColor, vec4(texColor.rgb, 1.0), 0.6);

            // Output the final color
            color = mixedColor;
        } else {
            color = fragColor;
        }
    }
)";


#endif //PROG2002_GRID_H

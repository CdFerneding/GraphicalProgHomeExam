#include <string>
#ifndef HOMEEXAM_GRID_H
#define HOMEEXAM_GRID_H

// Vertex and fragment shader source code
const std::string VS_Grid = R"(
    #version 430 core
    layout(location = 0) in vec3 position;
    layout(location = 1) in vec4 color;
    layout(location = 2) in vec2 texCoords;

    out vec2 fragTexCoords;
    out vec4 fragColor;
    // for diffuse lighting
    out vec3 Normal;
    out vec3 FragPos;

    uniform mat4 u_Model;
    uniform mat4 u_View;
    uniform mat4 u_Projection;

    void main()
    {
        gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0);
        fragTexCoords = texCoords;
        fragColor = color;

        vec3 upNormal = vec3(0.0, 0.0, 1.0);
        FragPos = vec3(u_Model * vec4(position, 1.0));
        Normal = upNormal;
    }
)";

const std::string FS_Grid = R"(
    #version 430 core

    in vec2 fragTexCoords;
    in vec4 fragColor;

    in vec3 FragPos; 
    in vec3 Normal; 

    uniform sampler2D u_Texture;
    uniform float u_TextureState;
    uniform vec3 u_LightColor;
    uniform vec3 u_LightPosition;
    uniform vec3 u_ViewPos;

    out vec4 color;
    
    void main()
    {
        // ambient lighting
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * u_LightColor;

        // diffuse lighting
        vec3 norm = normalize(Normal); // in case of scaling
        vec3 lightDir = normalize(u_LightPosition - FragPos); 
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * u_LightColor;

        // specular lighting
        float specularStrength = 0.5;
        vec3 viewDir = normalize(u_ViewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256); // last param (of pow) definies the "shininess": the higher the shinier
        vec3 specular = specularStrength * spec * u_LightColor;

        vec3 colorAfterLighting = (ambient + diffuse + specular) * fragColor.rgb;

        if(u_TextureState != 0.0f) {
            // Sample the texture using the texture coordinates
            vec4 texColor = texture(u_Texture, fragTexCoords);

            // Mix the color from the vertex attribute with the texture color
            // Adjust the alpha value manually
            vec4 colortmp = vec4(colorAfterLighting, fragColor.a);
            vec4 mixedColor = mix(fragColor, colortmp, 0.6);

            // Output the final color
            color = mixedColor;
        } else {
            color = vec4(colorAfterLighting, fragColor.a);
        }
    }
)";


#endif //HOMEEXAM_GRID_H

#include <string>
#ifndef HOMEEXAM_CUBE_H
#define HOMEEXAM_CUBE_H

// Vertex and fragment shader source code
const std::string VS_Cube = R"(
    #version 430 core
    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 aNormal;

    out vec3 TexCoords;
    // for diffuse lighting
    out vec3 Normal;
    out vec3 FragPos;

    uniform mat4 u_Model;
    uniform mat4 u_View;
    uniform mat4 u_Projection;

    void main()
    {;
        TexCoords = position;
        gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0);

        FragPos = vec3(u_Model * vec4(position, 1.0));
        Normal = aNormal;
    }
)";

const std::string FS_Cube = R"(
    #version 430 core

    in vec3 TexCoords;

    in vec3 Normal; 
    in vec3 FragPos; 
    
    uniform samplerCube CubeMap;
    uniform float u_Opacity;
    uniform float u_TextureState;

    uniform vec3 u_Color;
    uniform vec3 u_LightColor;
    uniform vec3 u_LightPosition;
    uniform vec3 u_ViewPos;
    uniform float u_LightIntensity;

    out vec4 color;
    
    void main()
    {
        // ambient lighting
        float ambientStrength = 0.3;
        vec3 ambient = ambientStrength * u_LightColor;

        // diffuse lighting
        vec3 norm = normalize(Normal); // in case of scaling
        vec3 lightDir = normalize(u_LightPosition - FragPos); // direction of fragment towards lightPos 
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * u_LightColor;

        // specular lighting
        float specularStrength = 0.5;
        vec3 viewDir = normalize(u_ViewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // last param (of pow) definies the "shininess": the higher the shinier
        vec3 specular = specularStrength * spec * u_LightColor;

        vec3 colorAfterLighting = (ambient + diffuse + specular) * u_Color * u_LightIntensity;

        if(u_TextureState != 0.0f) {
             //Sample the texture using the texture coordinates
             vec4 texColor = texture(CubeMap, TexCoords);
             color = mix(vec4(texColor.rgb, u_Opacity), vec4(colorAfterLighting, 1.0), 0.5);
        } else {
            color = vec4(colorAfterLighting, u_Opacity);
        }
    }
)";

#endif //HOMEEXAM_CUBE_H

#ifndef PROG2002_GEOMETRICTOOLS_H
#define PROG2002_GEOMETRICTOOLS_H

#include <array>
#include <vector>
#include "../../external/glm/glm/ext/matrix_transform.hpp"

namespace GeometricTools {
    constexpr std::array<float, 3*2> UnitTriangle2D = {-0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.5f}; // [2,3]

    constexpr std::array<float, 5*4> UnitSquare2D = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
    };

    auto UnitSquare2DDivider(const unsigned int divisions) {
        // Divide each value of UnitSquare2D by the value in divisions
        auto square = UnitSquare2D;
        std::vector<float> newSquare;
        for(float i : square) {
            newSquare.push_back(i/(float) divisions);
        }
        return newSquare;
    }

    constexpr std::array<unsigned int, 3> TopologyTriangle2D = {0, 1, 2}; // [3]

    constexpr std::array<unsigned int, 6> TopologySquare2D = {0, 1, 2, 0, 2, 3}; // [6]

    // Constexpr version that generates the geometry of the grid, including texture coordinates
    // The shape of the generated data is PPTTPPTTPPTT..., meaning two components for position
    // and two components for texture coordinates.
    auto UnitGridGeometry2DWTCoords(const unsigned int divisions) {
        std::vector<float> vertices;
        int count = 0;
        for (int i = 0; i <= divisions; ++i) {
            float xPos = (i / static_cast<float>(divisions)) * 2 - 1.0f;
            for (int j = 0; j <= divisions; ++j) {
                float yPos = (j / static_cast<float>(divisions)) * 2 - 1.0f;
                vertices.push_back(xPos);
                vertices.push_back(yPos);
                vertices.push_back(0.0f); // Set z-coordinate to 0 for a 2D grid
                //put the color in rgba format
                vertices.push_back(1.0);
                vertices.push_back(1.0);
                vertices.push_back(1.0);
                vertices.push_back(1.0);
                // texCoords
                vertices.push_back((xPos + 1) / 2);
                vertices.push_back((yPos + 1) / 2);
                count++;
                //std::cout << xPos << ", " << yPos << ", 1.0, 1.0, 1.0, 1.0, 1.0" << std::endl;
            }
        }
        for (int i = 0; i <= divisions; ++i) {
            float xPos = (i / static_cast<float>(divisions)) * 2 - 1.0f;
            for (int j = 0; j <= divisions; ++j) {
                float yPos = (j / static_cast<float>(divisions)) * 2 - 1.0f;
                vertices.push_back(xPos);
                vertices.push_back(yPos);
                vertices.push_back(0.0f); // Set z-coordinate to 0 for a 2D grid
                //put the color in rgba format
                vertices.push_back(0.0);
                vertices.push_back(0.0);
                vertices.push_back(0.0);
                vertices.push_back(1.0);
                //texCoords
                vertices.push_back((xPos + 1) / 2);
                vertices.push_back((yPos + 1) / 2);
                count++;
                //std::cout << xPos << ", " << yPos << ", 1.0, 1.0, 1.0, 1.0, 1.0" << std::endl;
            }
        }
        //convert grid to std array
        return vertices;
    }


    auto UnitGrid2D(const unsigned int divisions) {
        //Create a std vector of float with size of 3*divisions*divisions
        std::vector<float> vertices;
        for (int i = 0; i <= divisions; ++i) {
            float xPos = (i / static_cast<float>(divisions))*2 - 1.0f;
            for (int j = 0; j <= divisions; ++j) {
                float yPos = (j / static_cast<float>(divisions))*2 - 1.0f;
                vertices.push_back(xPos);
                vertices.push_back(yPos);
                vertices.push_back(0.0f); // Set z-coordinate to 0 for a 2D grid
            }
        }

        //convert grid to std array
        return vertices;

    }

    auto UnitGrid2DWithColor(const unsigned int divisions) {
        std::vector<float> vertices;
        int count = 0;
        for (int i = 0; i <= divisions; ++i) {
            float xPos = (i / static_cast<float>(divisions))*2 - 1.0f;
            for (int j = 0; j <= divisions; ++j) {
                float yPos = (j / static_cast<float>(divisions))*2 - 1.0f;
                vertices.push_back(xPos);
                vertices.push_back(yPos);
                vertices.push_back(0.0f); // Set z-coordinate to 0 for a 2D grid
                //put the color in rgba format
                vertices.push_back(1.0);
                vertices.push_back(1.0);
                vertices.push_back(1.0);
                vertices.push_back(1.0);
                count++;
                //std::cout << xPos << ", " << yPos << ", 1.0, 1.0, 1.0, 1.0, 1.0" << std::endl;
            }
        }
        for (int i = 0; i <= divisions; ++i) {
            float xPos = (i / static_cast<float>(divisions))*2 - 1.0f;
            for (int j = 0; j <= divisions; ++j) {
                float yPos = (j / static_cast<float>(divisions))*2 - 1.0f;
                vertices.push_back(xPos);
                vertices.push_back(yPos);
                vertices.push_back(0.0f); // Set z-coordinate to 0 for a 2D grid
                //put the color in rgba format
                vertices.push_back(0.0);
                vertices.push_back(0.0);
                vertices.push_back(0.0);
                vertices.push_back(1.0);
                count++;
                //std::cout << xPos << ", " << yPos << ", 1.0, 1.0, 1.0, 1.0, 1.0" << std::endl;
            }
        }
        //convert grid to std array
        return vertices;
    }

    auto UnitGrid2DTopology(const unsigned int divisions) {

        std::vector<unsigned int> indices;

        // Generate indices for triangles that form the grid
        bool isWhite;
        for (unsigned int i = 0; i < divisions; i++) {
            for (unsigned int j = 0; j < divisions; j++) {
                //(divisions+1) * (divisions+1)
                // Calculate the indices for the four vertices of each quad
                isWhite = (i%2==j%2);

                //It will add 384
                unsigned int topLeft = i * (divisions + 1) + j;
                unsigned int topRight = topLeft + 1;
                unsigned int bottomLeft = (i + 1) * (divisions + 1) + j;
                unsigned int bottomRight = bottomLeft + 1;

                // Define two triangles for each quad
                // Triangle 1: top left -> top right -> bottom left
                indices.push_back(topLeft+ (isWhite ? 81: 0));
                indices.push_back(topRight+ (isWhite ? 81: 0));
                indices.push_back(bottomLeft+ (isWhite ? 81: 0));

                // Triangle 2: top right -> bottom right -> bottom left
                indices.push_back(topRight+ (isWhite ? 81: 0));
                indices.push_back(bottomRight+ (isWhite ? 81: 0));
                indices.push_back(bottomLeft+ (isWhite ? 81: 0));
                //std::cout << topLeft << " " << topRight << " " << bottomLeft << " " << topRight << " " << bottomRight << " " << bottomLeft << std::endl;
            }
        }

        return indices;
    }

    auto UnitGrid2DTopologyLab4(const unsigned int divisions) {

        std::vector<unsigned int> indices;

        for (unsigned int i = 0; i < divisions; i++) {
            for (unsigned int j = 0; j < divisions; j++) {
                //(divisions+1) * (divisions+1)
                // Calculate the indices for the four vertices of each quad

                //It will add 384
                unsigned int topLeft = i * (divisions + 1) + j;
                unsigned int topRight = topLeft + 1;
                unsigned int bottomLeft = (i + 1) * (divisions + 1) + j;
                unsigned int bottomRight = bottomLeft + 1;

                // Define two triangles for each quad
                // Triangle 1: top left -> top right -> bottom left
                indices.push_back(topLeft);
                indices.push_back(topRight);
                indices.push_back(bottomLeft);

                // Triangle 2: top right -> bottom right -> bottom left
                indices.push_back(topRight);
                indices.push_back(bottomRight);
                indices.push_back(bottomLeft);
                //std::cout << topLeft << " " << topRight << " " << bottomLeft << " " << topRight << " " << bottomRight << " " << bottomLeft << std::endl;
            }
        }

        return indices;
    }

    constexpr std::array<float, 3 * 8> UnitCube = {
            -0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f
    };

    constexpr std::array<float, 8 * 7> UnitCubeWithColor(float r, float g, float b, float a) {
        return {
                -0.5f, -0.5f,  0.0f, r, g, b, a,
                0.5f, -0.5f,  0.0f, r, g, b, a,
                0.5f,  0.5f,  0.0f, r, g, b, a,
                -0.5f,  0.5f,  0.0f, r, g, b, a,
                -0.5f, -0.5f, 1.0f, r, g, b, a,
                0.5f, -0.5f, 1.0f, r, g, b, a,
                0.5f,  0.5f, 1.0f, r, g, b, a,
                -0.5f,  0.5f, 1.0f, r, g, b, a
        };
    }

    constexpr std::array<unsigned int, 36> CubeTopology = {
            0, 1, 2,
            2, 3, 0,
            4, 5, 6,
            6, 7, 4,
            4, 7, 3,
            3, 0, 4,
            1, 5, 6,
            6, 2, 1,
            3, 2, 6,
            6, 7, 3,
            0, 1, 5,
            5, 4, 0
    };

    /* rotating a cube that has 7 attributes (3 of which being positions) */
    auto rotateCube(std::vector<float> cube, float angleX, float rotationAngleY, float rotationAngleZ) {

        std::vector<float> newCube;

        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotationAngleY), glm::vec3(0.0f, 1.0f, 0.0f));
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotationAngleZ), glm::vec3(0.0f, 0.0f, 1.0f));

        for (int i = 0; i < cube.size(); i+=7) {
            glm::vec4 vertex = glm::vec4(cube[i], cube[i+1], cube[i+2], 1.0f);
            vertex = rotationMatrix * vertex;
            newCube.push_back(vertex.x);
            newCube.push_back(vertex.y);
            newCube.push_back(vertex.z);
            newCube.push_back(cube[i+3]);
            newCube.push_back(cube[i+4]);
            newCube.push_back(cube[i+5]);
            newCube.push_back(cube[i+6]);
        }

        return newCube;
    }

    /* rotate a cube (number of attributes not hardcoded) */
    auto rotateCubeGeneric(std::vector<float> cube, float angleX, float rotationAngleY, float rotationAngleZ, unsigned int numberOfAttributes) {

        std::vector<float> newCube;

        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotationAngleY), glm::vec3(0.0f, 1.0f, 0.0f));
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotationAngleZ), glm::vec3(0.0f, 0.0f, 1.0f));

        for (int i = 0; i < cube.size(); i += numberOfAttributes) {
            glm::vec4 vertex = glm::vec4(cube[i], cube[i + 1], cube[i + 2], 1.0f);
            vertex = rotationMatrix * vertex;
            newCube.push_back(vertex.x); // attribute position 0
            newCube.push_back(vertex.y); // attribute position 1
            newCube.push_back(vertex.z); // attribute position 2
            unsigned int attributePosition = 3;
            while (attributePosition < numberOfAttributes) {
                newCube.push_back(cube[i + attributePosition]);
                attributePosition += 1;
            }
        }

        return newCube;
    }

    /* translate a cube that has 7 attributes (3 of which being positions) */
    std::vector<float> translateCube(std::vector<float> cube, float x, float y, float z) {
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
        std::vector<float> newCube;
        for (int i = 0; i < cube.size(); i+=7) {
            glm::vec4 vertex = glm::vec4(cube[i], cube[i+1], cube[i+2], 1.0f);
            vertex = translationMatrix * vertex;
            newCube.push_back(vertex.x);
            newCube.push_back(vertex.y);
            newCube.push_back(vertex.z);
            newCube.push_back(cube[i+3]);
            newCube.push_back(cube[i+4]);
            newCube.push_back(cube[i+5]);
            newCube.push_back(cube[i+6]);
        }
        return newCube;
    }

    /* translate a cube (number of attributes not hardcoded) */
    std::vector<float> translateCubeGeneric(std::vector<float> cube, float x, float y, float z, unsigned int numberOfAttributes) {
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
        std::vector<float> newCube;
        for (int i = 0; i < cube.size(); i += numberOfAttributes) {
            glm::vec4 vertex = glm::vec4(cube[i], cube[i + 1], cube[i + 2], 1.0f);
            vertex = translationMatrix * vertex;
            newCube.push_back(vertex.x);
            newCube.push_back(vertex.y);
            newCube.push_back(vertex.z);
            unsigned int attributePosition = 3;
            while (attributePosition < numberOfAttributes) { 
                newCube.push_back(cube[i + attributePosition]); 
                attributePosition += 1; 
            }
        }
        return newCube;
    }
}


#endif //PROG2002_GEOMETRICTOOLS_H

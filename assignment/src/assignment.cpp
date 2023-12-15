#include "assignment.h"
// shader objects
#include "shaders/grid.h"
#include "shaders/square.h"
#include "shaders/unit.h"
// rendering framework
#include "GeometricTools.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertextArray.h"
#include "Shader.h"
#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"
#include "TextureManager.h"
#include <RenderCommands.h>
// libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>

AssignementApplication* AssignementApplication::current_application = nullptr;

AssignementApplication::AssignementApplication(const std::string& name, const std::string& version,
    unsigned int width, unsigned int height) : GLFWApplication(name, version, width, height) {

    currentXSelected = 0;
    currentYSelected = 0;

    hasMoved = false;
    moveUnitFrom = {-1, -1};
    isUnitSelected = false;

    toggleTexture = 0.0f;
}

AssignementApplication::~AssignementApplication() = default;


void AssignementApplication::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
        // move selection square
        case GLFW_KEY_UP:
            getAssignementApplication()->move(UP);
            break;
        case GLFW_KEY_DOWN:
            getAssignementApplication()->move(DOWN);
            break;
        case GLFW_KEY_LEFT:
            getAssignementApplication()->move(LEFT);
            break;
        case GLFW_KEY_RIGHT:
            getAssignementApplication()->move(RIGHT);
            break;

        // handle square movement
        // There is two key for up and left because we are using a QWERTY and an AZERTY keyboard
        case GLFW_KEY_Z:
        case GLFW_KEY_W:
		    getAssignementApplication()->move(UP);
		    break;
        case GLFW_KEY_A:
            getAssignementApplication()->move(LEFT);
            break;
        case GLFW_KEY_S:
            getAssignementApplication()->move(DOWN);
            break;
        case GLFW_KEY_D:
            getAssignementApplication()->move(RIGHT);
            break;
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        // exit program
        case GLFW_KEY_Q:
            getAssignementApplication()->exit();
            break;
        // handle camera changes
        case GLFW_KEY_P:
            getAssignementApplication()->zoom(1.0f);
            break;
        case GLFW_KEY_O:
            getAssignementApplication()->zoom(-1.0f);
            break;
        case GLFW_KEY_L:
            getAssignementApplication()->rotate(5);
            break;
        case GLFW_KEY_H:
            getAssignementApplication()->rotate(-5);
            // counter-clockwise rotation
            break;
        // handle unit selection
        case GLFW_KEY_ENTER:
            getAssignementApplication()->select();
            break;
        // toggle texturing/blending
        case GLFW_KEY_T:
            getAssignementApplication()->setTextureState();
            break;
        default:
            break;
        }
    }
}

void AssignementApplication::setTextureState() {
    toggleTexture = (toggleTexture == 0.0f) ? 1.0f : 0.0f;
}

void AssignementApplication::select() {
    isUnitSelected = true;
}

void AssignementApplication::move(Direction direction) {
    hasMoved = true;
    switch (direction) {
    case UP:
        currentYSelected = (currentYSelected + 1) % numberOfSquare;
        break;
    case DOWN:
        currentYSelected = (currentYSelected - 1) % numberOfSquare;
        break;
    case LEFT:
        currentXSelected = (currentXSelected + 1) % numberOfSquare;
        break;
    case RIGHT:
        currentXSelected = (currentXSelected - 1) % numberOfSquare;
        break;
    default:
        hasMoved = false;
        break;
    }
}

void AssignementApplication::rotate(float degree) {
    camera.rotateArroundLookAt(degree);
}

void AssignementApplication::zoom(float zoomValue) {
    camera.zoom(zoomValue);
}

std::vector<float> AssignementApplication::createSquare(float opacity) const {
    float sideLength = 2.0f / static_cast<float>(numberOfSquare);
    float halfSideLength = sideLength * 0.5f;

    std::vector<float> newSelectionSquare = {
            -halfSideLength, -halfSideLength, 0.0f, // 0.0f, 1.0f, 0.0f, opacity,
            -halfSideLength, +halfSideLength, 0.0f, // 0.0f, 1.0f, 0.0f, opacity,
            +halfSideLength, +halfSideLength, 0.0f, // 0.0f, 1.0f, 0.0f, opacity,
            +halfSideLength, -halfSideLength, 0.0f, // 0.0f, 1.0f, 0.0f, opacity
    };

    return newSelectionSquare;
}

std::vector<float> AssignementApplication::createUnit() const {
    float sideLength = 2.0f / static_cast<float>(numberOfSquare);
    float halfSideLength = sideLength * 0.5f;

    // cube centered around origin in relation to the gridSquareSize
    std::vector<float> selectionCube = {
        -halfSideLength, -halfSideLength, -halfSideLength,
        -halfSideLength, halfSideLength, -halfSideLength,
        +halfSideLength, halfSideLength, -halfSideLength,
        halfSideLength, -halfSideLength, -halfSideLength,
        -halfSideLength, -halfSideLength, halfSideLength,
        -halfSideLength, halfSideLength, halfSideLength,
        halfSideLength, halfSideLength, halfSideLength,
        halfSideLength, -halfSideLength, halfSideLength,
    };
    return selectionCube;
}

void AssignementApplication::setupUnits()
{
    // init red team
    for (unsigned int i = 0; i < numberOfSquare; i++) {
        for (unsigned int j = 0; j < 2; j++) {
            UnitInfo unitInfo;
            unitInfo.currentColor = colorTeam1;
            unitInfo.teamColor = unitInfo.currentColor;
            unitInfo.currentPosition = glm::vec2(j, i);
            unitInfo.selected = false;
            unitInfoVector.push_back(unitInfo);
        }
    }
    // init blue team
    for (unsigned int i = 0; i < numberOfSquare; i++) {
        for (unsigned int j = 0; j < 2; j++) {
            UnitInfo unitInfo;
            unitInfo.currentColor = colorTeam2;
            unitInfo.teamColor = unitInfo.currentColor;
            unitInfo.currentPosition = glm::vec2(numberOfSquare - 1 - j, i);
            unitInfo.selected = false;
            unitInfoVector.push_back(unitInfo);
        }
    }
}

int AssignementApplication::selectUnit()
{
    for (unsigned int outerIndex = 0; outerIndex < unitInfoVector.size(); outerIndex++) {
        // the the "moveunitfrom" position equals one unit.currentPosition we can move it to where the square currently is
        if (unitInfoVector[outerIndex].currentPosition[0] == currentXSelected && unitInfoVector[outerIndex].currentPosition[1] == currentYSelected) {
            // only select this unit if no other unit is selected yet
            bool selectOnlyOneUnit = true;
            for (unsigned int innerIndex = 0; innerIndex < unitInfoVector.size(); innerIndex++) {
                if (unitInfoVector[innerIndex].selected == true) {
                    selectOnlyOneUnit = false;
                }
            }
            // if no other selected unit has been found go further
            if (selectOnlyOneUnit) {
                moveUnitFrom = { static_cast<int>(currentXSelected), static_cast<int>(currentYSelected) };
                unitInfoVector[outerIndex].currentColor = colorUnitSelected;
                unitInfoVector[outerIndex].selected = true;
                std::cout << "unit selected." << std::endl;
                return 1;
            }
        }
    }
    return 0;
}

int AssignementApplication::moveUnit()
{
    // update unitInfoVector.currentPosition if a square registered "Enter" / "isUnitSelected" / unit is being moved
    for (unsigned int outerLoop = 0; outerLoop < unitInfoVector.size(); outerLoop++) {
        if (unitInfoVector[outerLoop].selected == true) {
            // check if any of the units occupy the current square
            for (unsigned int innerLoop = 0; innerLoop < unitInfoVector.size(); innerLoop++) {
                if (unitInfoVector[innerLoop].currentPosition[0] == currentXSelected && unitInfoVector[innerLoop].currentPosition[1] == currentYSelected) {
                    std::cout << "cannot move there. square is occupied." << std::endl;
                    unitInfoVector[outerLoop].selected = false;
                    // return 1 to not call selectUnit after this
                    // otherwise the unit that is currently blocking the movement would get selected
                    return 1;
                }
            }
            std::cout << "unit moved from: " << unitInfoVector[outerLoop].currentPosition[0] << ", " << unitInfoVector[outerLoop].currentPosition[1] << " to: " << currentXSelected << ", " << currentYSelected << std::endl;
            unitInfoVector[outerLoop].currentPosition = glm::vec2(currentXSelected, currentYSelected);
            unitInfoVector[outerLoop].currentColor = unitInfoVector[outerLoop].teamColor;
            moveUnitFrom = { -1, -1 };
            unitInfoVector[outerLoop].selected = false;
            return 1;
            
        }
    }
    return 0;
}


unsigned AssignementApplication::Run() {

    current_application = this; 

    //--------------------------------------------------------------------------------------------------------------
    //
    //  define vertices and indices for the grid, selectionSquare and the unit
    //
    //--------------------------------------------------------------------------------------------------------------
    auto gridVertices = GeometricTools::UnitGridGeometry2DWTCoords(numberOfSquare);
    auto gridIndices = GeometricTools::UnitGrid2DTopology(numberOfSquare);

    auto squareVertices = createSquare(1.0f);
    auto squareIndices = GeometricTools::TopologySquare2D;

    auto unitVertices = createUnit();
    auto unitIndices = GeometricTools::CubeTopology;


    //--------------------------------------------------------------------------------------------------------------
    //
    //  define the layout for the grid, selectionSquare and the unit
    //
    //--------------------------------------------------------------------------------------------------------------

    // grid Layout
    auto gridLayout = std::make_shared<BufferLayout>(BufferLayout({
        {ShaderDataType::Float3, "position", false},
        {ShaderDataType::Float4, "color", false},
        {ShaderDataType::Float2, "texCoords", false}
        }));

    // selectionSquareLayout
    auto squareLayout = std::make_shared<BufferLayout>(BufferLayout({
        {ShaderDataType::Float3, "position", false}, 
        }));

    // cube Layout
    auto unitLayout = std::make_shared<BufferLayout>(BufferLayout({
        {ShaderDataType::Float3, "position", false},
        }));

    //--------------------------------------------------------------------------------------------------------------
    //
    //  prepping for the grid and the cube
    //
    //--------------------------------------------------------------------------------------------------------------

    // VAO Grid
    auto VAO_Grid = std::make_shared<VertexArray>();
    VAO_Grid->Bind();
    auto VBO_Grid = std::make_shared<VertexBuffer>(gridVertices.data(), sizeof(float) * gridVertices.size());
    VBO_Grid->SetLayout(*gridLayout);
    VAO_Grid->AddVertexBuffer(VBO_Grid);
    auto IBO_Grid = std::make_shared<IndexBuffer>(gridIndices.data(), static_cast<GLsizei>(gridIndices.size()));
    VAO_Grid->SetIndexBuffer(IBO_Grid); 

    // VAO SelectionSquare
    auto VAO_Square = std::make_shared<VertexArray>();
    VAO_Square->Bind(); 
    auto VBO_Square = std::make_shared<VertexBuffer>(squareVertices.data(),
        sizeof(float) * squareVertices.size()); 
    VBO_Square->SetLayout(*squareLayout); 
    VAO_Square->AddVertexBuffer(VBO_Square); 
    auto IBO_Square = std::make_shared<IndexBuffer>(squareIndices.data(), 
        static_cast<GLsizei>(squareIndices.size()));
    VAO_Square->SetIndexBuffer(IBO_Square);

    // VAO Unit
    auto VAO_Unit = std::make_shared<VertexArray>();
    VAO_Unit->Bind();
    auto VBO_Unit = std::make_shared<VertexBuffer>(unitVertices.data(),
        sizeof(float) * unitVertices.size());
    VBO_Unit->SetLayout(*unitLayout);
    VAO_Unit->AddVertexBuffer(VBO_Unit);
    auto IBO_Unit = std::make_shared<IndexBuffer>(unitIndices.data(),
        static_cast<GLsizei>(unitIndices.size()));
    VAO_Unit->SetIndexBuffer(IBO_Unit);


    //--------------------------------------------------------------------------------------------------------------
    //
    // Shader module
    //
    //--------------------------------------------------------------------------------------------------------------
    // grid shader
    auto* shaderGrid = new Shader(VS_Grid, FS_Grid);
    shaderGrid->Bind();
    // square shader
    auto* shaderSquare = new Shader(VS_Square, FS_Square);
    shaderSquare->Bind();
    // cube shader
    auto* shaderUnit = new Shader(VS_Unit, FS_Unit);
    shaderUnit->Bind();


    //--------------------------------------------------------------------------------------------------------------
    //
    // camera
    //
    //--------------------------------------------------------------------------------------------------------------
    // Use PerspectiveCamera class instead
    camera = PerspectiveCamera(
        PerspectiveCamera::Frustrum{ glm::radians(45.0f), 1.0f, 1.0f, 1.0f, 10.0f }, // frustrum
        glm::vec3(0.0f, -3.0f, 2.0f), // camera position
        glm::vec3(0.0f, 0.0f, 0.0f), // lookAt
        glm::vec3(0.0f, 0.0f, 1.0f) // upVector
    );



    //--------------------------------------------------------------------------------------------------------------
    //
    // Texture module
    //
    //--------------------------------------------------------------------------------------------------------------
    TextureManager* textureManager = TextureManager::GetInstance(); 
    // Load 2D texture for the grid
    bool success2D = textureManager->LoadTexture2DRGBA("gridTexture", "resources/textures/floor_texture.png", 0, true);
    if (!success2D) {
        std::cout << "2D Texture not loaded correctly." << std::endl;
    }
    // Give the textures to the shader
    GLuint gridTexture = textureManager->GetUnitByName("gridTexture"); 

    // Load Cube Map
    //TODO: loading the wood texture (all textures exept black-tile) does not work for the cubemap (whyever that might be).
    bool successCube = textureManager->LoadCubeMapRGBA("cubeTexture", "resources/textures/cube_texture.png", 1, true);
    if (!successCube) {
        std::cout << "Cube Map not loaded correctly." << std::endl;
    }
    GLuint unitTexture = textureManager->GetUnitByName("cubeTexture"); 


    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    //--------------------------------------------------------------------------------------------------------------
    //
    // start execution
    //
    // important notice:
    // for semi-transparancy (blending) to work opaque objects have to be drawn first!
    // Also OpenGL Depth-Test functionality might interfere
    //--------------------------------------------------------------------------------------------------------------
    // Renderloop variables
    glm::mat4 unitModel = glm::mat4(1.0f);
    glm::mat4 squareModel = glm::mat4(1.0f);

    hasMoved = true;
    bool setup = true; // for units in first iteration
    bool isOccupied = false;

    // initialization the units
    setupUnits();
    while (!glfwWindowShouldClose(window))
    {

        //preparation of Window and Shader
        RenderCommands::SetClearColor(0.663f, 0.663f, 0.663f, 1.0f); // grey background
        RenderCommands::Clear();

        // handle input
        glfwSetKeyCallback(window, AssignementApplication::key_callback);

        //--------------------------------------------------------------------------------------------------------------
        //
        // grid processing
        //
        //--------------------------------------------------------------------------------------------------------------
        VAO_Grid->Bind();
        shaderGrid->Bind();
        shaderGrid->UploadUniformMatrix4fv("u_Model", camera.GetViewProjectionMatrix());
        shaderGrid->UploadUniformMatrix4fv("u_View", camera.GetViewMatrix());
        shaderGrid->UploadUniformMatrix4fv("u_Projection", camera.GetProjectionMatrix());
        shaderGrid->UploadUniformFloat1("u_TextureState", static_cast<float>(toggleTexture));
        shaderGrid->UploadUniform1i("u_Texture", gridTexture);
        RenderCommands::DrawIndex(GL_TRIANGLES, VAO_Grid); 
        
        //--------------------------------------------------------------------------------------------------------------
        //
        // unit processing
        //
        //--------------------------------------------------------------------------------------------------------------
        // update unitColor when enter is hit on a cube
        if (isUnitSelected) {
            // callmove unit first otherwise the selected unit would instantly try to move to the current square 
            // which is occupied (by itself) and therefore canceling the move
            if (!moveUnit()) {
                selectUnit();
            }
            isUnitSelected = false;
        }


        // draw all 32 cubes according to their currentPosition and selected state
        for (unsigned int unitIndex = 0; unitIndex < unitInfoVector.size(); unitIndex++) {
            // unit information
            glm::vec3 currentUnitColor;
            currentUnitColor = unitInfoVector[unitIndex].teamColor;
            if (unitInfoVector[unitIndex].selected == true) {
                currentUnitColor = colorUnitSelected;
            } else if (unitInfoVector[unitIndex].currentPosition[0] == currentXSelected && unitInfoVector[unitIndex].currentPosition[1] == currentYSelected) {
                currentUnitColor = colorUnitHover;
            }
            glm::vec2 currentPosition = unitInfoVector[unitIndex].currentPosition;

            // helper
            float sideLength = 2.0f / static_cast<float>(numberOfSquare);
            float targetXOffset = sideLength / 2 + 3 * sideLength;
            float targetYOffset = sideLength / 2 - 4 * sideLength;

            // transforming and scaling units
            unitModel = glm::mat4(1.0f);
            float translationX = targetXOffset - sideLength * currentPosition[0];
            float translationY = targetYOffset + sideLength * currentPosition[1];
            unitModel = glm::translate(unitModel, glm::vec3(translationX, translationY, sideLength / 2));
            unitModel = glm::scale(unitModel, glm::vec3(0.56, 0.7, 0.56));

            // bind Unit Buffers, upload unit uniforms and draw unit
            // range: [0,1]
            float unitOpacity = 0.8;
            
            VAO_Unit->Bind();
            shaderUnit->Bind();
            shaderUnit->UploadUniformMatrix4fv("u_Model", unitModel * camera.GetViewProjectionMatrix());
            shaderUnit->UploadUniformMatrix4fv("u_View", camera.GetViewMatrix());
            shaderUnit->UploadUniformMatrix4fv("u_Projection", camera.GetProjectionMatrix());
            shaderUnit->UploadUniformFloat1("u_TextureState", static_cast<float>(toggleTexture));
            shaderUnit->UploadUniformFloat1("u_Opacity", unitOpacity);
            shaderUnit->UploadUniformFloat3("u_Color", currentUnitColor);
            shaderUnit->UploadUniform1i("CubeMap", unitTexture);
            RenderCommands::DrawIndex(GL_TRIANGLES, VAO_Unit);
            }
        

        //--------------------------------------------------------------------------------------------------------------
        //
        // square processing
        //
        //--------------------------------------------------------------------------------------------------------------
        glm::vec3 squareColor = glm::vec3(0.0f, 1.0f, 0.0f);
        float squareOpacity = 1.0f;
        // check if the current square has a unit on it
        for (unsigned int unitIndex = 0; unitIndex < unitInfoVector.size(); unitIndex++) {
            if (unitInfoVector[unitIndex].currentPosition[0] == currentXSelected && unitInfoVector[unitIndex].currentPosition[1] == currentYSelected) {
                squareOpacity = 0.0f;
                break;
            }
        }
        if (hasMoved) {
            // helper
            float sideLength = 2.0f / static_cast<float>(numberOfSquare);
            // HINT: board position (0,0) is the bottom Right
            float bottomRight = numberOfSquare * sideLength - 1;
            
            // translating square
            squareModel = glm::mat4(1.0f);
            // HINT: from our perspective the x movement works inverted in relation to the board coordinates
            float translationX = bottomRight - sideLength/2 - currentXSelected * sideLength;
            float translationY = bottomRight + sideLength / 2 - sideLength * numberOfSquare + currentYSelected * sideLength;
            squareModel = glm::translate(squareModel, glm::vec3(translationX, translationY, 0.001));
            
            hasMoved = false;
        }

        // bind square buffer, upload square uniforms and draw square
        VAO_Square->Bind();
        shaderSquare->Bind();
        shaderSquare->UploadUniformMatrix4fv("u_Model", squareModel * camera.GetViewProjectionMatrix());
        shaderSquare->UploadUniformMatrix4fv("u_View", camera.GetViewMatrix());
        shaderSquare->UploadUniformMatrix4fv("u_Projection", camera.GetProjectionMatrix());
        shaderSquare->UploadUniformFloat3("u_Color", squareColor);
        shaderSquare->UploadUniformFloat1("u_Opacity", squareOpacity);
        RenderCommands::DrawIndex(GL_TRIANGLES, VAO_Square);

        // Swap front and back buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //--------------------------------------------------------------------------------------------------------------
    //
    // OpenGL cleanup
    //
    // theoretically not neccessary since destructors should be called automatically after variables go out of scope
    // but you never know with OpenGL
    //--------------------------------------------------------------------------------------------------------------

    // Cleanup Grid Buffers
    shaderGrid->~Shader();
    VAO_Grid->~VertexArray();
    VBO_Grid->~VertexBuffer();
    IBO_Grid->~IndexBuffer();

    // Cleanup Square Buffers
    shaderSquare->~Shader();
    VAO_Square->~VertexArray();
    VBO_Square->~VertexBuffer();
    IBO_Square->~IndexBuffer();

    // Cleanup Unit Buffers
    shaderUnit->~Shader();
    VAO_Unit->~VertexArray();
    VBO_Unit->~VertexBuffer();
    IBO_Unit->~IndexBuffer();

    return stop();
}

unsigned AssignementApplication::stop() {
    unsigned code = GLFWApplication::stop();

    if (GLFWApplication::stop() != EXIT_SUCCESS) {

        //print error message
        std::cerr << "Error stopping GLFW application" << std::endl;
        std::cerr << "Error code: " << code << std::endl;

        //print opengl message
        std::cerr << "OpenGL Error: " << glGetError() << std::endl;
    }
    return code;
}

AssignementApplication* AssignementApplication::getAssignementApplication() {
    return AssignementApplication::current_application;
}

void AssignementApplication::exit() {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

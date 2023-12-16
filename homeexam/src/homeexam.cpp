#include "homeexam.h"
// shader objects
#include "shaders/grid.h"
#include "shaders/cube.h"
#include "shaders/sun.h"
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
#include <iostream>
// std::time should be standart cpp function, but the build pipeline does not seem to find it so:
#include <ctime>
#include <cstdlib> 

HomeExamApplication* HomeExamApplication::current_application = nullptr;

HomeExamApplication::HomeExamApplication(const std::string& name, const std::string& version,
    unsigned int width, unsigned int height) : GLFWApplication(name, version, width, height) {

    currentXSelected = 0;
    currentYSelected = 0;

    hasMoved = false;
    moveCounter = 0;

    toggleTexture = 0.0f;
}

HomeExamApplication::~HomeExamApplication() = default;


void HomeExamApplication::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
            // move selection square
        case GLFW_KEY_UP:
            getHomeExamApplication()->move(UP);
            break;
        case GLFW_KEY_DOWN:
            getHomeExamApplication()->move(DOWN);
            break;
        case GLFW_KEY_LEFT:
            getHomeExamApplication()->move(LEFT);
            break;
        case GLFW_KEY_RIGHT:
            getHomeExamApplication()->move(RIGHT);
            break;

        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        // exit program
        case GLFW_KEY_Q:
            getHomeExamApplication()->exit();
            break;
        // handle camera changes
        case GLFW_KEY_S:
            getHomeExamApplication()->zoom(1.0f);
            break;
        case GLFW_KEY_W:
            getHomeExamApplication()->zoom(-1.0f);
            break;
        case GLFW_KEY_D:
            getHomeExamApplication()->rotate(5);
            break;
        case GLFW_KEY_A:    
            getHomeExamApplication()->rotate(-5);
            // counter-clockwise rotation
            break;
        case GLFW_KEY_T:
            getHomeExamApplication()->setTextureState();
            break;
        default:
            break;
        }
    }
}

void HomeExamApplication::setTextureState() {
    toggleTexture = (toggleTexture == 0.0f) ? 1.0f : 0.0f;
}

/*current X Selected goes from 0 to numberOfSquares-1 -> [0,9]
  same for Y Selected. 
 */
void HomeExamApplication::move(Direction direction) {
    hasMoved = true;
    TileInfo nextTile;
    TileInfo* nextTileAfter = nullptr;
    switch (direction) {
    case UP:
        // +1 does not need to be checked (for index out of bounds exeption) because of the ring of walls around the grids
        nextTile = GridState[currentXSelected * 10 + currentYSelected + 1];
        if (currentYSelected < 8) {
            nextTileAfter = &GridState[currentXSelected * 10 + currentYSelected + 2];
        }
        if (nextTile.hasObstacle || nextTile.hasPillar) break;
        if (nextTile.hasBox) {
            if (nextTileAfter == nullptr) {
                break;
            }
            else if (!nextTileAfter->hasBox && !nextTileAfter->hasObstacle && !nextTileAfter->hasPillar) {
                GridState[currentXSelected * 10 + currentYSelected + 1].hasBox = false;
                GridState[currentXSelected * 10 + currentYSelected + 2].hasBox = true;
            }
            else {
                break;
            }
        }
        currentYSelected++;
        moveCounter++;
        break;
    case DOWN:
        nextTile = GridState[currentXSelected * 10 + currentYSelected - 1];
        if (currentYSelected > 1) {
            nextTileAfter = &GridState[currentXSelected * 10 + currentYSelected - 2];
        }
        if (nextTile.hasObstacle || nextTile.hasPillar) break;
        if (nextTile.hasBox) {
            if (nextTileAfter == nullptr) {
                break;
            }
            else if (!nextTileAfter->hasBox && !nextTileAfter->hasObstacle && !nextTileAfter->hasPillar) {
                GridState[currentXSelected * 10 + currentYSelected - 1].hasBox = false;
                GridState[currentXSelected * 10 + currentYSelected - 2].hasBox = true;
            }
            else {
                break;
            }
        }
        currentYSelected--;
        moveCounter++;
        break;
    case LEFT:
        nextTile = GridState[(currentXSelected + 1) * 10 + currentYSelected];
        if (currentXSelected < 8) {
            nextTileAfter = &GridState[(currentXSelected + 2) * 10 + currentYSelected];
        }
        if (nextTile.hasObstacle || nextTile.hasPillar) break;
        if (nextTile.hasBox) {
            if (nextTileAfter == nullptr) {
                break;
            }
            else if (!nextTileAfter->hasBox && !nextTileAfter->hasObstacle && !nextTileAfter->hasPillar) {
                GridState[(currentXSelected + 1) * 10 + currentYSelected].hasBox = false;
                GridState[(currentXSelected + 2) * 10 + currentYSelected].hasBox = true;
            }
            else {
                break;
            }
        }
        currentXSelected++;
        moveCounter++;
        break;
    case RIGHT:
        nextTile = GridState[(currentXSelected - 1) * 10 + currentYSelected];
        if (currentXSelected > 1) {
            nextTileAfter = &GridState[(currentXSelected - 2) * 10 + currentYSelected];
        }
        if (nextTile.hasObstacle || nextTile.hasPillar) break;
        if (nextTile.hasBox) {
            if (nextTileAfter == nullptr) {
                break;
            }
            else if (!nextTileAfter->hasBox && !nextTileAfter->hasObstacle && !nextTileAfter->hasPillar) {
                GridState[(currentXSelected - 1) * 10 + currentYSelected].hasBox = false;
                GridState[(currentXSelected - 2) * 10 + currentYSelected].hasBox = true;
            }
            else {
                break;
            }
        }
        currentXSelected--;
        moveCounter++;
        break;
    default:
        hasMoved = false;
        break;
    }
}

void HomeExamApplication::rotate(float degree) {
    camera.rotateArroundLookAt(degree);
}

void HomeExamApplication::zoom(float zoomValue) {
    camera.zoom(zoomValue);
}



void HomeExamApplication::setupWarehouse(int numOfPillars, int numOfBoxes, int numOfBoxDest)
{
    // Seed the random number generator with the current time
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 10; ++y) {
            TileInfo tile;
            tile.currentPos = glm::vec2(x, y);
            // walls
            if (x == 0 || x == 9 || y == 0 || y == 9) {
                tile.hasObstacle = true;
            }
            else {
                tile.hasObstacle = false;
            }
            tile.hasBox = false;
            tile.isBoxFinished = false;
            tile.hasPillar = false;
            tile.hasBoxDest = false;
            GridState.push_back(tile);
        }
    }
    // place the pillars randomly (within the warehouse walls --> 9x9)
    int pillarCounter = 0;
    while (pillarCounter < numOfPillars) {
        int randomX = rand() % 8 + 1;
        int randomY = rand() % 8 + 1;
        int tileIndex = randomX * 10 + randomY;
        if (!GridState[tileIndex].hasObstacle && !GridState[tileIndex].hasPillar) {
            GridState[tileIndex].hasPillar = true;
            pillarCounter++;

            // Erase the occupied Tile from the freeTile array
            //freeTiles.erase(freeTiles.begin()+tileIndex);
        }
    }
    // place boxes
    int boxCounter = 0;
    while (boxCounter < numOfBoxes) {
        // make box the only object in the game that spawns within 6x6 grid to make the game solvable (since you are only allowed to push boxes)
        int randomX = rand() % 6 + 2;
        int randomY = rand() % 6 + 2;
        int tileIndex = randomX * 10 + randomY;
        if (!GridState[tileIndex].hasObstacle && !GridState[tileIndex].hasPillar && !GridState[tileIndex].hasBox) {
            GridState[tileIndex].hasBox = true;
            boxCounter++;

            // Erase the occupied Tile from the freeTile array
            //freeTiles.erase(freeTiles.begin() + tileIndex);
        }
    }
    // place box destinations
    int boxDestCounter = 0;
    while (boxDestCounter < numOfBoxes) { // same number of box destinations as boxes
        int randomX = rand() % 8 + 1;
        int randomY = rand() % 8 + 1;
        int tileIndex = randomX * 10 + randomY;
        if (!GridState[tileIndex].hasObstacle && !GridState[tileIndex].hasPillar && !GridState[tileIndex].hasBox && !GridState[tileIndex].hasBoxDest) {
            GridState[tileIndex].hasBoxDest = true; 
            boxDestCounter++;

            // Erase the occupied Tile from the freeTile array
            // freeTiles.erase(freeTiles.begin() + tileIndex);  not the same index as GridState
        }
    }
    
    // place player in the warehouse
    bool wasPlayerPlaced = false;
    while (!wasPlayerPlaced) {
        // number between 1 and 8 (rand()%8 -> [0,7]) for tile index (the same as currently Selected variables)
        int randomX = rand() % 8 + 1;
        int randomY = rand() % 8 + 1;
        int tileIndex = randomX * 10 + randomY;
        if (!GridState[tileIndex].hasObstacle && !GridState[tileIndex].hasPillar && !GridState[tileIndex].hasBox && !GridState[tileIndex].hasBoxDest) {
            currentXSelected = randomX;
            currentYSelected = randomY;
            wasPlayerPlaced = true;
        }
    }
}

// outdated / not used. I'll leave it here just in case
int HomeExamApplication::findTile(glm::vec2 checkingPos, std::vector<TileInfo> tileGrid)
{
    int counter = 0;
    for (TileInfo tile : tileGrid) {
        if (tile.currentPos == checkingPos) {
            return counter;
        }
        else counter++;
    }
    return counter; 
}

std::vector<float> HomeExamApplication::unitCubeGeometry() const {
    float sideLength = 2.0f / static_cast<float>(numberOfSquare);
    float halfSideLength = sideLength * 0.5f;

    // cube centered around origin in relation to the gridSquareSize
    std::vector<float> selectionCube = {
        /*-halfSideLength, -halfSideLength, -halfSideLength,
        -halfSideLength, halfSideLength, -halfSideLength,
        +halfSideLength, halfSideLength, -halfSideLength,
        halfSideLength, -halfSideLength, -halfSideLength,
        -halfSideLength, -halfSideLength, halfSideLength,
        -halfSideLength, halfSideLength, halfSideLength,
        halfSideLength, halfSideLength, halfSideLength,
        halfSideLength, -halfSideLength, halfSideLength,*/


        -halfSideLength, -halfSideLength, -halfSideLength, 0.0f, 0.0f, -1.0f,
        halfSideLength, -halfSideLength, -halfSideLength, 0.0f, 0.0f, -1.0f,
        halfSideLength, halfSideLength, -halfSideLength, 0.0f, 0.0f, -1.0f,
        halfSideLength, halfSideLength, -halfSideLength, 0.0f, 0.0f, -1.0f,
        -halfSideLength, halfSideLength, -halfSideLength, 0.0f, 0.0f, -1.0f,
        -halfSideLength, -halfSideLength, -halfSideLength, 0.0f, 0.0f, -1.0f,

        -halfSideLength, -halfSideLength, halfSideLength, 0.0f, 0.0f, 1.0f,
        halfSideLength, -halfSideLength, halfSideLength, 0.0f, 0.0f, 1.0f,
        halfSideLength, halfSideLength, halfSideLength, 0.0f, 0.0f, 1.0f,
        halfSideLength, halfSideLength, halfSideLength, 0.0f, 0.0f, 1.0f,
        -halfSideLength, halfSideLength, halfSideLength, 0.0f, 0.0f, 1.0f,
        -halfSideLength, -halfSideLength, halfSideLength, 0.0f, 0.0f, 1.0f,

        -halfSideLength, halfSideLength, halfSideLength, -1.0f, 0.0f, 0.0f,
        -halfSideLength, halfSideLength, -halfSideLength, -1.0f, 0.0f, 0.0f,
        -halfSideLength, -halfSideLength, -halfSideLength, -1.0f, 0.0f, 0.0f,
        -halfSideLength, -halfSideLength, -halfSideLength, -1.0f, 0.0f, 0.0f,
        -halfSideLength, -halfSideLength, halfSideLength, -1.0f, 0.0f, 0.0f,
        -halfSideLength, halfSideLength, halfSideLength, -1.0f, 0.0f, 0.0f,

        halfSideLength, halfSideLength, halfSideLength, 1.0f, 0.0f, 0.0f,
        halfSideLength, halfSideLength, -halfSideLength, 1.0f, 0.0f, 0.0f,
        halfSideLength, -halfSideLength, -halfSideLength, 1.0f, 0.0f, 0.0f,
        halfSideLength, -halfSideLength, -halfSideLength, 1.0f, 0.0f, 0.0f,
        halfSideLength, -halfSideLength, halfSideLength, 1.0f, 0.0f, 0.0f,
        halfSideLength, halfSideLength, halfSideLength, 1.0f, 0.0f, 0.0f,

        -halfSideLength, -halfSideLength, -halfSideLength, 0.0f, -1.0f, 0.0f,
        halfSideLength, -halfSideLength, -halfSideLength, 0.0f, -1.0f, 0.0f,
        halfSideLength, -halfSideLength, halfSideLength, 0.0f, -1.0f, 0.0f,
        halfSideLength, -halfSideLength, halfSideLength, 0.0f, -1.0f, 0.0f,
        -halfSideLength, -halfSideLength, halfSideLength, 0.0f, -1.0f, 0.0f,
        -halfSideLength, -halfSideLength, -halfSideLength, 0.0f, -1.0f, 0.0f,

        -halfSideLength, halfSideLength, -halfSideLength, 0.0f, 1.0f, 0.0f,
        halfSideLength, halfSideLength, -halfSideLength, 0.0f, 1.0f, 0.0f,
        halfSideLength, halfSideLength, halfSideLength, 0.0f, 1.0f, 0.0f,
        halfSideLength, halfSideLength, halfSideLength, 0.0f, 1.0f, 0.0f,
        -halfSideLength, halfSideLength, halfSideLength, 0.0f, 1.0f, 0.0f,
        -halfSideLength, halfSideLength, -halfSideLength, 0.0f, 1.0f, 0.0f
    };
    return selectionCube;
}


unsigned HomeExamApplication::Run() {

    current_application = this;

    //--------------------------------------------------------------------------------------------------------------
    //
    //  define vertices and indices for the grid, selectionSquare and the unit
    //
    //--------------------------------------------------------------------------------------------------------------
    auto gridVertices = GeometricTools::UnitGridGeometry2DWTCoords(numberOfSquare);
    auto gridIndices = GeometricTools::UnitGrid2DTopology(numberOfSquare);

    auto cubeVertices = unitCubeGeometry();
    auto cubeIndices = GeometricTools::CubeTopology;


    //--------------------------------------------------------------------------------------------------------------
    //
    //  define the layout for the grid, selectionSquare and the unit
    //
    //--------------------------------------------------------------------------------------------------------------

    // grid Layout
    auto gridLayout = std::make_shared<BufferLayout>(BufferLayout({
        {ShaderDataType::Float3, "position", false},
        {ShaderDataType::Float4, "color", false},
        {ShaderDataType::Float2, "texCoords", false},
        }));

    // cube Layout
    auto cubeLayout = std::make_shared<BufferLayout>(BufferLayout({
        {ShaderDataType::Float3, "position", false},
        {ShaderDataType::Float3, "normal", false} // the norm vector (used for diffuse lighting)
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

    // VAO Unit
    auto VAO_Cube = std::make_shared<VertexArray>();
    VAO_Cube->Bind();
    auto VBO_Cube = std::make_shared<VertexBuffer>(cubeVertices.data(),
        sizeof(float) * cubeVertices.size());
    VBO_Cube->SetLayout(*cubeLayout);
    VAO_Cube->AddVertexBuffer(VBO_Cube);
    auto IBO_Cube = std::make_shared<IndexBuffer>(cubeIndices.data(),
        static_cast<GLsizei>(cubeIndices.size()));
    VAO_Cube->SetIndexBuffer(IBO_Cube);


    //--------------------------------------------------------------------------------------------------------------
    //
    // Shader setup for the grid and cube
    //
    //--------------------------------------------------------------------------------------------------------------
    // grid shader
    auto* shaderGrid = new Shader(VS_Grid, FS_Grid);
    shaderGrid->Bind();

    // cube shader
    auto* shaderCube = new Shader(VS_Cube, FS_Cube);
    shaderCube->Bind();

    // sun shader
    auto* shaderSun = new Shader(VS_Sun, FS_Sun);
    shaderSun->Bind();

    //--------------------------------------------------------------------------------------------------------------
    //
    // camera
    //
    //--------------------------------------------------------------------------------------------------------------
    // Use PerspectiveCamera class instead
    camera = PerspectiveCamera(
        PerspectiveCamera::Frustrum{ glm::radians(45.0f), 1.0f, 1.0f, 1.0f, 10.0f }, // frustrum
        glm::vec3(0.0f, -1.8f, 2.4f), // camera position
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
    glm::mat4 cubeModel = glm::mat4(1.0f);

    hasMoved = true;
    bool setup = true; // for units in first iteration
    bool isGameWon = false;

    setupWarehouse();

    // lighting variables
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f); 
    // place the light a bit to the right of where the player is looking from (at the start)
    glm::vec3 lightPosition = glm::vec3(1.2f, 0.0f, 1.2f);
    float ambientStrength = 1.0f;

    while (!glfwWindowShouldClose(window))
    {

        //preparation of Window and Shader
        RenderCommands::SetClearColor(0.663f, 0.663f, 0.663f, 1.0f); // grey background
        RenderCommands::Clear();

        // handle input
        glfwSetKeyCallback(window, HomeExamApplication::key_callback);

        //--------------------------------------------------------------------------------------------------------------
        //
        // lighting
        //
        //--------------------------------------------------------------------------------------------------------------

        // inside the corresponding shaders I added:
        // 
        // ambient lighting (simple form of global illumination)
        // global illumination describes the fact that objects will often still have some light
        // even if the don't see the light source directly due to reflection on other objects in the scene
        // 
        // diffuse lighting 
        // lets a fragment shine brither the close it is to the light source: calculates the angle between vertex and light source
        // the (non-uniform) scaling I am using does not affect the normal vectors because all objects are cubes. 
        // The normal vectors for the grid shader will always be up (assuming the light source is above the grid))
        //
        // specular lighting takes the viewer Position into account
        
        // change the light's position values over time (glfwGetTime returns the elapsed time in seconds)
        float radius = 2.0f;
        float rotationSpeed = 0.15f;
        lightPosition.x = radius * cos(rotationSpeed * glfwGetTime());
        lightPosition.z = radius * sin(rotationSpeed * glfwGetTime());

        // set lightIntensity in relation to the sun's apex
        if (ambientStrength >= 0.5) ambientStrength = (lightPosition.z + 2) / 4;


        //--------------------------------------------------------------------------------------------------------------
        //
        // warehouse processing
        //
        //--------------------------------------------------------------------------------------------------------------
        VAO_Grid->Bind();
        shaderGrid->Bind();
        shaderGrid->UploadUniformMatrix4fv("u_Model", camera.GetViewProjectionMatrix());
        shaderGrid->UploadUniformMatrix4fv("u_View", camera.GetViewMatrix());
        shaderGrid->UploadUniformMatrix4fv("u_Projection", camera.GetProjectionMatrix());
        shaderGrid->UploadUniformFloat1("u_TextureState", static_cast<float>(toggleTexture));
        shaderGrid->UploadUniformFloat1("u_AmbientStrength", ambientStrength);
        shaderGrid->UploadUniformFloat3("u_LightColor", lightColor); 
        shaderGrid->UploadUniformFloat3("u_LightPosition", lightPosition);
        shaderGrid->UploadUniformFloat3("u_ViewPos", camera.GetPosition());
        shaderGrid->UploadUniform1i("u_Texture", gridTexture);
        RenderCommands::DrawIndex(GL_TRIANGLES, VAO_Grid);

        // draw all obstacles, boxes and box Destinations according to the vector GridState
        for (int tileIndex = 0; tileIndex < GridState.size(); tileIndex++) {
            TileInfo tile = GridState[tileIndex];
            // unit information
            glm::vec3 currentColor;
            float unitOpacity = 1;

            glm::vec2 currentPosition = tile.currentPos;

            // helper
            float sideLength = 2.0f / static_cast<float>(numberOfSquare);
            float targetXOffset = sideLength / 2 + 4 * sideLength;
            float targetYOffset = sideLength / 2 - 5 * sideLength;

            cubeModel = glm::mat4(1.0f);
            float translationX = targetXOffset - sideLength * currentPosition[0];
            float translationY = targetYOffset + sideLength * currentPosition[1];

            // colloring, transforming and scaling units according to GridState
            // box destinations
            if (tile.hasBoxDest) {
                currentColor = boxDestColor; 
                cubeModel = glm::translate(cubeModel, glm::vec3(translationX, translationY, 0));
                cubeModel = glm::scale(cubeModel, glm::vec3(0.8, 0.8, 0.1)); 
            }
            // boxes
            if (tile.hasBox) {
                currentColor = boxColor;
                cubeModel = glm::translate(cubeModel, glm::vec3(translationX, translationY, sideLength / 4));
                cubeModel = glm::scale(cubeModel, glm::vec3(0.8, 0.8, 0.6));
            }
            // walls
            if (tile.hasObstacle) {
                currentColor = wallsColor;
                unitOpacity = 0.98;
                cubeModel = glm::translate(cubeModel, glm::vec3(translationX, translationY, sideLength / 2));
                cubeModel = glm::scale(cubeModel, glm::vec3(0.98, 0.98, 1.0)); 
            }
            // pillars
            if (tile.hasPillar) {
                currentColor = pillarCollor;
                cubeModel = glm::translate(cubeModel, glm::vec3(translationX, translationY, sideLength / 2));
                cubeModel = glm::scale(cubeModel, glm::vec3(0.5, 0.5, 1.0));
            }
            // box on correct spot
            if (tile.hasBox && tile.hasBoxDest) {
                cubeModel = glm::mat4(1.0f); 
                cubeModel = glm::translate(cubeModel, glm::vec3(translationX, translationY, sideLength / 4)); 
                cubeModel = glm::scale(cubeModel, glm::vec3(0.8, 0.8, 0.6)); 
                currentColor = boxCorrectPosColor;
            }
            // empty tiles
            if (!tile.hasObstacle && !tile.hasPillar && !tile.hasBox && !tile.hasBoxDest) {
                continue;
            }

            VAO_Cube->Bind();
            shaderCube->Bind();
            shaderCube->UploadUniformMatrix4fv("u_Model", cubeModel * camera.GetViewProjectionMatrix());
            shaderCube->UploadUniformMatrix4fv("u_View", camera.GetViewMatrix());
            shaderCube->UploadUniformMatrix4fv("u_Projection", camera.GetProjectionMatrix());
            shaderCube->UploadUniformFloat1("u_TextureState", static_cast<float>(toggleTexture));
            shaderCube->UploadUniformFloat1("u_AmbientStrength", ambientStrength);
            shaderCube->UploadUniformFloat1("u_Opacity", unitOpacity);
            shaderCube->UploadUniformFloat3("u_Color", currentColor);
            shaderCube->UploadUniformFloat3("u_LightColor", lightColor); 
            shaderCube->UploadUniformFloat3("u_LightPosition", lightPosition);
            shaderCube->UploadUniformFloat3("u_ViewPos", camera.GetPosition());
            shaderCube->UploadUniform1i("CubeMap", unitTexture);
            RenderCommands::DrawIndex(GL_TRIANGLES, VAO_Cube);
        }

        //--------------------------------------------------------------------------------------------------------------
        //
        // player processing
        //
        //--------------------------------------------------------------------------------------------------------------
        float squareOpacity = 1.0f;

        // helper
        float sideLength = 2.0f / static_cast<float>(numberOfSquare);
        // HINT: board position (0,0) is the bottom Right
        float bottomRight = numberOfSquare * sideLength - 1;

        // translating square
        cubeModel = glm::mat4(1.0f);
        // HINT: from our perspective the x movement works inverted in relation to the board coordinates
        float translationX = bottomRight - sideLength / 2 - currentXSelected * sideLength;
        float translationY = bottomRight + sideLength / 2 - sideLength * numberOfSquare + currentYSelected * sideLength;
        cubeModel = glm::translate(cubeModel, glm::vec3(translationX, translationY, 0.001));
        cubeModel = glm::scale(cubeModel, glm::vec3(0.8, 0.8, 0.9));

        // bind square buffer, upload square uniforms and draw square
        VAO_Cube->Bind();
        shaderCube->Bind();
        shaderCube->UploadUniformMatrix4fv("u_Model", cubeModel * camera.GetViewProjectionMatrix());
        shaderCube->UploadUniformMatrix4fv("u_View", camera.GetViewMatrix());
        shaderCube->UploadUniformMatrix4fv("u_Projection", camera.GetProjectionMatrix());
        shaderCube->UploadUniformFloat1("u_AmbientStrength", ambientStrength);
        shaderCube->UploadUniformFloat3("u_Color", playerColor);
        shaderCube->UploadUniformFloat3("u_LightColor", lightColor);
        shaderCube->UploadUniformFloat3("u_LightPosition", lightPosition); 
        shaderCube->UploadUniformFloat3("u_ViewPos", camera.GetPosition()); 
        shaderCube->UploadUniformFloat1("u_Opacity", squareOpacity);
        RenderCommands::DrawIndex(GL_TRIANGLES, VAO_Cube);

        //--------------------------------------------------------------------------------------------------------------
        //
        // processing sun (white cube)
        //
        //--------------------------------------------------------------------------------------------------------------

        // translating and scaling the sun
        cubeModel = glm::mat4(1.0f);
        
        cubeModel = glm::translate(cubeModel, lightPosition);
        cubeModel = glm::scale(cubeModel, glm::vec3(0.4));
        
        // bind square buffer, upload square uniforms and draw square
        VAO_Cube->Bind();
        shaderSun->Bind();
        shaderSun->UploadUniformMatrix4fv("u_Model", cubeModel* camera.GetViewProjectionMatrix());
        shaderSun->UploadUniformMatrix4fv("u_View", camera.GetViewMatrix());
        shaderSun->UploadUniformMatrix4fv("u_Projection", camera.GetProjectionMatrix());
        RenderCommands::DrawIndex(GL_TRIANGLES, VAO_Cube); 

        //--------------------------------------------------------------------------------------------------------------
        //
        // check wincondition
        //
        //--------------------------------------------------------------------------------------------------------------
        int correctBoxCounter = 0;
        for (TileInfo tile : GridState) {
            if (tile.hasBox && tile.hasBoxDest) correctBoxCounter++;
            if (correctBoxCounter == 6 && !isGameWon) 
            {
                std::cout << "Won Game with " << moveCounter << " moves!" << std::endl;
                isGameWon = true;
            }
        }

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

    // Cleanup Unit Buffers
    shaderCube->~Shader();
    VAO_Cube->~VertexArray();
    VBO_Cube->~VertexBuffer();
    IBO_Cube->~IndexBuffer();

    return stop();
}

unsigned HomeExamApplication::stop() {
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

HomeExamApplication* HomeExamApplication::getHomeExamApplication() {
    return HomeExamApplication::current_application;
}

void HomeExamApplication::exit() {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

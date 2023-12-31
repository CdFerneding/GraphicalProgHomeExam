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
    TileInfo nextTile;
    TileInfo* nextTileAfter = nullptr;
    switch (direction) {
    case UP:
        // +1 does not need to be checked (for index out of bounds exeption) because of the ring of walls around the grids
        nextTile = GridState[currentXSelected * 10 + currentYSelected + 1];
        if (currentYSelected < 8) {
            nextTileAfter = &GridState[currentXSelected * 10 + currentYSelected + 2];
        }
        if (nextTile.hasObstacle || nextTile.hasPillar) break; // cancel movement if next block is an obstacle
        if (nextTile.hasBox) { // the next tile is a box. The following lines check if the box can be moved
            if (nextTileAfter == nullptr) {
                break;
            }
            else if (!nextTileAfter->hasBox && !nextTileAfter->hasObstacle && !nextTileAfter->hasPillar) {
                GridState[currentXSelected * 10 + currentYSelected + 1].hasBox = false;
                GridState[currentXSelected * 10 + currentYSelected + 2].hasBox = true;
            }
            else {
                break; // cancel movement if the tile behind the box has a box, wall or pillar standing on it
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
        
        // restrict pillar spawning to ensure the game is solvable. No threat of outOfBounds because there will always be the walls.
        // this checks if pillars are already existent in a ring around the current tile (pillar candidate)
        // this eliminates the possibility of pillars trapping a box or box destination
        if (GridState[tileIndex + 1].hasPillar || GridState[tileIndex - 1].hasPillar 
            || GridState[tileIndex - 10].hasPillar || GridState[tileIndex - 9].hasPillar || GridState[tileIndex - 11].hasPillar
            || GridState[tileIndex + 10].hasPillar || GridState[tileIndex + 9].hasPillar || GridState[tileIndex + 11].hasPillar)
            continue;

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
        // this prevents boxes from spawning next to a wall of the warehouse
        int randomX = rand() % 6 + 2;
        int randomY = rand() % 6 + 2;
        int tileIndex = randomX * 10 + randomY;

        // ensure boxes spawn does not make the game unsolvable
        // this checks a square with includes the current tileposition: if a box spawns in a square like formation... 
        // together with other boxes or pillars the formation is unmovable therefore breaking the game
        // this is extremely hard to test as this spawn has a low change of happening
        // It may be that I wont include this test in the final solution as it is computationally intensive (on game start) while not being incredibly rewarding
        // it also does not break the game so I will keep it
        if ((GridState[tileIndex + 1].hasBox || GridState[tileIndex + 1].hasPillar) &&
            (GridState[tileIndex + 10].hasBox || GridState[tileIndex + 10].hasPillar) &&
            (GridState[tileIndex + 11].hasBox || GridState[tileIndex + 11].hasPillar) &&
            (GridState[tileIndex - 1].hasBox || GridState[tileIndex - 1].hasPillar) &&
            (GridState[tileIndex + 10].hasBox || GridState[tileIndex + 10].hasPillar) &&
            (GridState[tileIndex + 9].hasBox || GridState[tileIndex + 9].hasPillar) &&
            (GridState[tileIndex + 1].hasBox || GridState[tileIndex + 1].hasPillar) &&
            (GridState[tileIndex - 10].hasBox || GridState[tileIndex - 10].hasPillar) &&
            (GridState[tileIndex - 11].hasBox || GridState[tileIndex - 11].hasPillar) &&
            (GridState[tileIndex - 10].hasBox || GridState[tileIndex - 10].hasPillar) &&
            (GridState[tileIndex - 9].hasBox || GridState[tileIndex - 9].hasPillar))
        {
            continue;
        }

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
    //  define vertices and indices for the grid and cube
    //
    //--------------------------------------------------------------------------------------------------------------
    auto gridVertices = GeometricTools::UnitGridGeometry2DWTCoords(numberOfSquare);
    auto gridIndices = GeometricTools::UnitGrid2DTopology(numberOfSquare);

    auto cubeVertices = unitCubeGeometry();
    auto cubeIndices = GeometricTools::CubeTopology;


    //--------------------------------------------------------------------------------------------------------------
    //
    //  define the layout for the grid and cube
    //
    //--------------------------------------------------------------------------------------------------------------

    // grid layout
    auto gridLayout = std::make_shared<BufferLayout>(BufferLayout({
        {ShaderDataType::Float3, "position", false},
        {ShaderDataType::Float4, "color", false},
        {ShaderDataType::Float2, "texCoords", false},
        }));

    // cube layout
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

    // VAO Cube
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
    std::unique_ptr<Shader> shaderGrid = std::make_unique<Shader>(VS_Grid, FS_Grid);
    std::unique_ptr<Shader> shaderCube = std::make_unique<Shader>(VS_Cube, FS_Cube);
    std::unique_ptr<Shader> shaderSun = std::make_unique<Shader>(VS_Sun, FS_Sun);

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

    // Load Rune Cube Map
    bool successCube = textureManager->LoadCubeMapRGBA("runeTexture", "resources/textures/cube_texture.png", 1, true);
    if (!successCube) {
        std::cout << "Cube Map not loaded correctly." << std::endl;
    }
    GLuint runeCubeMap = textureManager->GetUnitByName("runeTexture");

    // Load black marmor Cube Map
    successCube = textureManager->LoadCubeMapRGBA("blackMarmorTexture", "resources/textures/black-tile.jpg", 2, true);
    if (!successCube) {
        std::cout << "Cube Map not loaded correctly." << std::endl;
    }
    GLuint blackMarmorCubeMap = textureManager->GetUnitByName("blackMarmorTexture");

    // Load wood Cube Map
    successCube = textureManager->LoadCubeMapRGBA("woodTexture", "resources/textures/floor_texture.png", 3, true);
    if (!successCube) {
        std::cout << "Cube Map not loaded correctly." << std::endl;
    }
    GLuint woodCubeMap = textureManager->GetUnitByName("woodTexture");


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
            // upload corresponding cubemap
            if (tile.hasPillar || tile.hasObstacle)
                shaderCube->UploadUniform1i("CubeMap", blackMarmorCubeMap);
            else if (tile.hasBox)
                shaderCube->UploadUniform1i("CubeMap", woodCubeMap);
            else
                shaderCube->UploadUniform1i("CubeMap", runeCubeMap);
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
        
        // bind buffer, upload uniforms and draw 
        VAO_Cube->Bind();
        shaderSun->Bind();
        shaderSun->UploadUniformMatrix4fv("u_Model", cubeModel* camera.GetViewProjectionMatrix());
        shaderSun->UploadUniformMatrix4fv("u_View", camera.GetViewMatrix());
        shaderSun->UploadUniformMatrix4fv("u_Projection", camera.GetProjectionMatrix());
        RenderCommands::DrawIndex(GL_TRIANGLES, VAO_Cube); 

        //--------------------------------------------------------------------------------------------------------------
        //
        // check win condition
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

#ifndef HOMEEXAM_LAB3APPLICATION_H
#define HOMEEXAM_LAB3APPLICATION_H

#include <string>
#include <vector>
#include <unordered_map>
#include <array>
#include <memory>
#include "GLFWApplication.h"
#include "VertextArray.h"
#include "Shader.h"
#include "PerspectiveCamera.h"
#include <glm/glm.hpp>

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class HomeExamApplication : public GLFWApplication {
private:
    struct TileInfo {
        // x and y position: [0,9]
        glm::vec2 currentPos;
        bool hasObstacle;
        bool hasPillar;
        bool hasBox;
        bool hasBoxDest;
        bool isBoxFinished;
    };
    std::vector<TileInfo> GridState; 
    glm::vec3 boxColor = glm::vec3(181.0f /255.0f, 101.0f /255.0f, 29.0f /255.0f); // light brown
    glm::vec3 boxCorrectPosColor = glm::vec3(1.0f, 1.0f, 0.0f); // yellow
    glm::vec3 boxDestColor = glm::vec3(0.0f, 1.0f, 0.0f); // green
    // color for walls and pillars
    glm::vec3 wallsColor = glm::vec3(54.0f / 255.0f, 34.0f / 255.0f, 4.0f / 255.0f); // dark brown
    glm::vec3 pillarCollor = glm::vec3(54.0f / 255.0f, 34.0f / 255.0f, 50.0f / 255.0f);
    glm::vec3 playerColor = glm::vec3(0.0f, 0.0f, 1.0f);
    
    void setupWarehouse(int numOfPillars = 6, int numOfBoxes = 6, int numOfBoxDest = 6);
    int findTile(glm::vec2 checkingPos, std::vector<TileInfo> tileGrid);

    unsigned int currentXSelected; // Current x position of the selector
    unsigned int currentYSelected; // Current y position of the selector

    PerspectiveCamera camera; //The perspective camera used

    bool hasMoved; // True when the player is moving the selection square
    bool isUnitSelected;
    float toggleTexture; // toggle functionality to activate/deactivate textures and blending

    std::array<int, 2> moveUnitFrom; // Contain the previous position of the selected cube, (-1,-1) otherwise

    const unsigned int numberOfSquare = 10; // The number of square on the grid

    static HomeExamApplication* current_application; // The current_application used for the communication with the key_callback

    /**
    function to handle units
    */
    std::vector<float> unitCubeGeometry() const;

    // function to toggle texture State
    void setTextureState();

public:
    explicit HomeExamApplication(const std::string& name = "homeexam", const std::string& version = "0.0.1",
        unsigned int width = 1024, unsigned int height = 1024);

    ~HomeExamApplication();

    unsigned Run() override;
    unsigned stop() override;

    /**
     * Move the selection square in a specific direction
     * @param direction The direction to move the selection square
     */
    void move(Direction direction);

    /**
     * Function called when the player press any key on the keyboard
     */
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    /**
     * Function called when the key callback want to use a function of the current_application
     * @return current_application
     */
    static HomeExamApplication* getHomeExamApplication();

    /**
     * Function called by the key_callback when the player try to zoom in or out.
     * This function change the fov of the camera
     * @param zoomValue The value to add to the current fov
     */
    void zoom(float zoomValue);

    /**
     * Function called by the key_callback when the player try to rotate the camera around the lookAt point
     * It change the position of the camera
     * @param degree The value to add to the current rotation
     */
    void rotate(float degree);

    /**
     * Function called by the key_callback when the player want to exit the application
     */
    void exit();
};



#endif //HOMEEXAM_LAB3APPLICATION_H

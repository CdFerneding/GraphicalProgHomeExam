#ifndef PROG2002_LAB3APPLICATION_H
#define PROG2002_LAB3APPLICATION_H

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

class AssignementApplication : public GLFWApplication {
private:

    // struct to store information about each unit
    struct UnitInfo {
        glm::vec3 teamColor;
        glm::vec3 currentColor;
        glm::vec2 currentPosition;
        bool selected;
    };
    std::vector<UnitInfo> unitInfoVector;
    // unit color variables
    glm::vec3 colorTeam1 = glm::vec3(1.0, 0.0, 0.0); // red
    glm::vec3 colorTeam2 = glm::vec3(0.0, 0.0, 1.0); // blue
    glm::vec3 colorUnitSelected = glm::vec3(1.0f, 1.0f, 0.0f);
    glm::vec3 colorUnitHover = glm::vec3(0.0f, 1.0f, 0.0f);

    unsigned int currentXSelected; // Current x position of the selector
    unsigned int currentYSelected; // Current y position of the selector

    PerspectiveCamera camera; //The perspective camera used

    bool hasMoved; // True when the player is moving the selection square
    bool isUnitSelected;
    float toggleTexture; // toggle functionality to activate/deactivate textures and blending

    std::array<int, 2> moveUnitFrom; // Contain the previous position of the selected cube, (-1,-1) otherwise

    const unsigned int numberOfSquare = 8; // The number of square on the grid

    static AssignementApplication* current_application; // The current_application used for the communication with the key_callback

    /**
     * Create the green selection square on the board according to the current X and Y position
     * @param opacity The opacity of the square (0 if there is a cube, 1 if there isn't a cube)
     * @return The content for the vertex buffer of the selection square
     */
    [[nodiscard]] std::vector<float> createSquare(float opacity) const;

    /** 
    function to handle units
    */
    std::vector<float> createUnit() const;
    void setupUnits(); 
    int selectUnit(); 
    int moveUnit();

    // function to toggle texture State
    void setTextureState();

public:
    explicit AssignementApplication(const std::string& name = "assignment", const std::string& version = "0.0.1",
        unsigned int width = 800, unsigned int height = 600);

    ~AssignementApplication();

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
    static AssignementApplication* getAssignementApplication();

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
     * Function called by the key_callback when the player try to select a cube
     */
    void select();

    /**
     * Function called by the key_callback when the player want to exit the application
     */
    void exit();
};



#endif //PROG2002_LAB3APPLICATION_H

#pragma once
#include "player.cpp"
#include "panel.cpp"
#include "list"
#include "vector"
#include "renderer.cpp"
#include "vector2.cpp"

class Game {
public:
    Game();
    void start();
    void update();
    void movePlayer(Vector2 direction);
    void movePlayerTo(Vector2 position);
    Panel getPanelAtPosition(Vector2 position);
    Renderer renderer;
    Vector2 getCharacterPosition(char character, Panel panel);
    std::list<Vector2> getAvailableDirections(Vector2 position);
private:
    Player player;
    std::list<Panel> panels;
    Panel currentPanel;
    Vector2 spawnPoint;
    std::list<Vector2> availableDirections;
    std::vector<Vector2> forwardDirections;
    std::vector<Vector2> getForwardDirections();
    std::vector<Vector2> getBackwardDirections();
    void moveForward();
    void moveBackward();

    bool isPathSelection = false;

    bool isUpPressed = false;
    bool isDownPressed = false;
    bool isUpJustPressed = false;
    bool isDownJustPressed = false;
};
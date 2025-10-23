#pragma once
#include "list"
#include "vector"
#include "vector2.cpp"
#include "player.cpp"
#include "enemy.h"
#include "panel.cpp"
#include "renderer.cpp"

class Game {
public:
    Game();
    void start();
    void update();
    void restartFromLastCheckpoint();
private:
    Renderer renderer;
    Player player;
    std::list<Panel> panels;
    Panel currentPanel;
    Vector2 spawnPoint;
    std::list<Enemy> enemies;
    
    bool isUpPressed = false;
    bool isDownPressed = false;
    bool isUpJustPressed = false;
    bool isDownJustPressed = false;
    
    bool isPathSelection = false;
    bool hasKey = false;
    
    void movePlayer(Vector2 direction);
    void movePlayerTo(Vector2 position);
    Panel getPanelAtPosition(Vector2 position);
    
    Vector2 getCharacterPosition(char character, Panel panel);
    std::list<Vector2> getCharacterPositions(char character, Panel panel);
    char getCharacterAtPosition(Vector2 position);
    char getCharacterAtPosition(Vector2 position, Panel panel);
    std::list<Vector2> getAvailableDirections(Vector2 position);
    std::list<Vector2> getAvailableEnemyDirections(Vector2 position);    
    std::vector<Vector2> getForwardDirections(Player player);
    std::vector<Vector2> getBackwardDirections(Player player);
    void moveForward();
    void moveBackward();

    double lastEnemyMovedTime;
    float enemyMoveTime = 100;
    double pathSelectionTime = 0;
    float pathSelectionDuration = 500.0;
    void moveEnemy(Enemy &enemy);
};
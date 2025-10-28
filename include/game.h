#pragma once
#include "list"
#include "vector"
#include "vector2.cpp"
#include "player.cpp"
#include "enemy.h"
#include "panel.cpp"
#include "renderer.cpp"
#include "ESP32Encoder.h"

class Game {
public:
    Game();
    void start();
    void update();
    void inputs();
    void restartFromLastCheckpoint();
    void moveForward();
    void moveBackward();
    void inputForward();
    void inputBackward();
    void endGame();
private:
    Renderer renderer;
    Player player;
    std::list<Panel> panels;
    Panel currentPanel;
    Vector2 spawnPoint;
    std::list<Enemy> enemies;
    std::map<char, std::list<Panel>> teleportationPoints;
    std::string teleportationCharacters = "abc";
    
    bool isUpPressed = false;
    bool isDownPressed = false;
    bool isUpJustPressed = false;
    bool isDownJustPressed = false;
    
    bool isPathSelection = false;
    bool hasKey = false;
    
    void movePlayer(Vector2 direction);
    void movePlayerTo(Vector2 position);
    Panel& getPanelAtPosition(Vector2 position);
    
    Vector2 getCharacterPosition(char character, const Panel &panel);
    void setCharacterAtPosition(Vector2 position, char value);
    std::list<Vector2> getCharacterPositions(char character, const Panel &panel);
    char getCharacterAtPosition(Vector2 position);
    char getCharacterAtPosition(Vector2 position, const Panel &panel);
    std::list<Vector2> getAvailableDirections(Vector2 position);
    std::list<Vector2> getAvailableEnemyDirections(Vector2 position);    
    std::vector<Vector2> getForwardDirections(Player player);
    std::vector<Vector2> getBackwardDirections(Player player);

    double lastEnemyMovedTime;
    float enemyMoveTime = 100;
    double pathSelectionTime = 0;
    float pathSelectionDuration = 500.0;
    void moveEnemy(Enemy &enemy);

    ESP32Encoder encoder;

    int lastEncoded = 0;
    int encoderCount = 0;
    int encoderErrors = 0;
    float lastHallPin1Time = 0.0;
    float lastHallPin2Time = 0.0;
    int sensorValue1 = 0;
    int sensorValue2 = 0;
    int lastSensorValue1 = 0;
    int lastSensorValue2 = 0;
    float lastUpdateTime = 0.0;
    const int hallPin1 = 3;  // Pin connected to the hall effect sensor
    const int hallPin2 = 9;  // Pin connected to the hall effect sensor
    float rlTime = 0;
    float lrTime = 0;
};
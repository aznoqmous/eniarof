#include "game.h"
#include "Arduino.h"
#include "vector2.h"
#include "panels.cpp"

const float TAU = 6.28318;

#define BUTTON_UP_PIN 6
#define BUTTON_DOWN_PIN 7

Game::Game() {
    player = Player();

    panels.push_back(Panel(0, 0, 0, 0, 0, 0));
    panels.push_back(Panel(64, 0, 0, 1 * 32, 0, 1));
    panels.push_back(Panel(0, 32, 0, 2 * 32, 0, 2));
    panels.push_back(Panel(0, 32, 0, 3 * 32, 0, 3));
    panels.push_back(Panel(0, 32, 0, 4 * 32, 0, 4));
    panels.push_back(Panel(0, 32, 0, 5 * 32, 0, 5));
    panels.push_back(Panel(0, 32, 0, 6 * 32, 0, 6));
    panels.push_back(Panel(0, 32, 0, 7 * 32, 0, 7));

    currentPanel = panels.front();
    renderer.renderPanel(currentPanel);
    spawnPoint = getCharacterPosition('0', currentPanel);

    pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
    pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
}

void Game::start() {
    // movePlayer(30, 30);
    
    // spawnPoint.x = 40;
    // spawnPoint.y = 6;
    // player.direction = Vector2(1,0);
    movePlayerTo(spawnPoint);


    for(Panel panel : panels) {
        renderer.renderPanel(panel);
    }
}

Vector2 Game::getCharacterPosition(char character, Panel panel) {
    int x = 0;
    int y = 0;
    for(std::string row: MAPS[panel.index]){
        x = 0;
        for(char cell : row){
            if(cell == character){
                return Vector2(x,y);
            }
            x++;
        }
        y++;
    }
}

void Game::movePlayer(Vector2 direction){
    player.lastPosition = player.position;
    movePlayerTo(player.position + direction);
}

void Game::movePlayerTo(Vector2 position) {
    player.position = position;
    if(player.position.x < currentPanel.offset_x || player.position.x >= currentPanel.offset_x + currentPanel.width ||
       player.position.y < currentPanel.offset_y || player.position.y >= currentPanel.offset_y + currentPanel.height) {
        currentPanel = getPanelAtPosition(player.position);
        renderer.renderPanel(currentPanel);
    }

    availableDirections = getAvailableDirections(Vector2(player.position.x, player.position.y));
    if(availableDirections.size() == 1){
        player.direction = availableDirections.front();
    }    
}

std::vector<Vector2> Game::getForwardDirections(){
    std::vector<Vector2> directions;
    for(Vector2 dir : availableDirections) {
        if(dir != Vector2(-player.direction.x, -player.direction.y)){
            directions.push_back(dir);
        }
    }
    return directions;
}
std::vector<Vector2> Game::getBackwardDirections(){
    std::vector<Vector2> directions;
    for(Vector2 dir : availableDirections) {
        if(dir != Vector2(player.direction.x, player.direction.y)){
            directions.push_back(dir);
        }
    }
    return directions;
}

void Game::moveForward(){
    forwardDirections = getForwardDirections();
    if(forwardDirections.size() == 1){
        player.direction = forwardDirections.front();
    }
    else {
        isPathSelection = true;
        return;
    }
    movePlayer(player.direction);
}

void Game::moveBackward(){
    forwardDirections = getBackwardDirections();
    Vector2 direction;
    if(forwardDirections.size() == 1){
        direction = forwardDirections.front();
        player.direction = Vector2(-direction.x, -direction.y);
    }
    else {
        isPathSelection = true;
        return;
    }
    movePlayer(direction);
}

Panel Game::getPanelAtPosition(Vector2 position){
    for(Panel panel : panels) {
        if(position.x >= panel.offset_x && position.x < panel.offset_x + panel.width &&
           position.y >= panel.offset_y && position.y < panel.offset_y + panel.height) {
            return panel;
        }
    }
    return currentPanel; // Return current panel if no other found
}

std::list<Vector2> Game::getAvailableDirections(Vector2 position) {
    std::list<Vector2> directions;
    std::list<Vector2> possibleDirections = {
        Vector2::UP,
        Vector2::DOWN,
        Vector2::LEFT,
        Vector2::RIGHT
    };
    for(Vector2 dir : possibleDirections) {
        Vector2 newPos = position + dir;
        Panel panel = getPanelAtPosition(position + dir);
        char cell = panel.getCharAtPosition(Vector2(newPos.x - panel.offset_x, newPos.y - panel.offset_y));
        if(cell == '*') {
            directions.push_back(dir);
        }
    }
    return directions;
}



void Game::update() {
    isUpJustPressed = false;
    if(!digitalRead(BUTTON_UP_PIN)){
        if(!isDownJustPressed && !isUpPressed){
            isUpJustPressed = true;
        }
        isUpPressed = true;
    }
    else {
        isUpPressed = false;
    }
    
    // renderer.clearPanel(currentPanel);
    // renderer.renderPanel(currentPanel);
    
    renderer.drawPanelPixel(
        Vector2(
            player.position.x - currentPanel.offset_x,
            player.position.y - currentPanel.offset_y
        ),
        currentPanel,
        renderer.matrix.color565(12,12,12)
    ); // erase player previous position

    renderer.drawPanelBorder(currentPanel, renderer.matrix.color565(0, 0, 0)); // Draw panel border in green
    
    if(isPathSelection){
        int pathIndex = int(millis() / 500.0) % forwardDirections.size();
        for(Vector2 dir: forwardDirections){
            renderer.drawPanelPixel(
                Vector2(
                    player.position.x + dir.x - currentPanel.offset_x,
                    player.position.y + dir.y - currentPanel.offset_y
                ),
                currentPanel,
                renderer.matrix.color565(12,12,12)
            );
        }
        if(isUpJustPressed){
            isPathSelection = false;
            player.direction = forwardDirections[pathIndex];
            movePlayer(player.direction);
        }
        // draw current path
        else {
            renderer.drawPanelPixel(
                Vector2(
                    player.position.x + forwardDirections[pathIndex].x - currentPanel.offset_x,
                    player.position.y + forwardDirections[pathIndex].y - currentPanel.offset_y
                ),
                currentPanel,
                renderer.matrix.color565(255,12,12)
            );
        }
        
    }
    else {
        if(!digitalRead(BUTTON_UP_PIN)){
            // movePlayer(player.direction);
            moveForward();
        }
        if(!digitalRead(BUTTON_DOWN_PIN)){
            moveBackward();
            // movePlayerTo(spawnPoint);
        }
    }

    renderer.drawPanelBorder(currentPanel, renderer.matrix.color565(12, 12, 0)); // Draw panel border in green
    
    renderer.drawPanelPixel(
        Vector2(
            player.position.x - currentPanel.offset_x,
            player.position.y - currentPanel.offset_y
        ),
        currentPanel,
        0xFFFF
    ); // Draw player as white pixel
    
    renderer.update();
}

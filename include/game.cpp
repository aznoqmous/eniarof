#include "game.h"
#include "Arduino.h"
#include "vector2.h"
#include "panel.h"
#include "panels.cpp"
#include "renderer.h"

#define BUTTON_UP_PIN 6
#define BUTTON_DOWN_PIN 7

const float TAU = 6.28318;
const int PANEL_WIDTH = 64;
const int PANEL_HEIGHT = 32;

Game::Game() {

    // Initialize Renderer
    uint8_t rgbPins[]  = {7, 8, 9, 10, 11, 12};
    uint8_t addrPins[] = {17, 18, 19, 20, 21};
    uint8_t clockPin   = 14;
    uint8_t latchPin   = 15;
    uint8_t oePin      = 16;
    renderer.matrix = Adafruit_Protomatter(
        64 * panels.size(),          // Width of matrix (or matrices, if tiled horizontally)
        3,           // Bit depth, 1-6
        1, rgbPins,  // # of matrix chains, array of 6 RGB pins for each
        4, addrPins, // # of address pins (height is inferred), array of pins
        clockPin, latchPin, oePin, // Other matrix control pins
        false       // No double-buffering here (see "doublebuffer" example)
    );
    
    ProtomatterStatus status = renderer.matrix.begin();
    Serial.print("Protomatter begin() status: ");
    Serial.println((int)status);
    if(status != PROTOMATTER_OK) {
        for(;;);
    }

    player = Player();

    for(Panel panel : panels){
        addPanel(panel);
    }

    currentPanel = panels.front();
    renderer.renderPanel(currentPanel);
    spawnPoint = getCharacterPosition('0', currentPanel);

    pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
    pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
}

void Game::addPanel(Panel panel){
    panel.width = panel.rotation != 0 ? PANEL_HEIGHT : PANEL_WIDTH;
    panel.height = panel.rotation != 0 ? PANEL_WIDTH : PANEL_HEIGHT;
    panel.index = panels.size();
    panel.physical_offset_x = panel.index * 64;
    panels.push_back(panel);
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
    for(std::string row: panel.map){
        x = 0;
        for(char cell : row){
            if(cell == character){
                return Vector2(x,y);
            }
            x++;
        }
        y++;
    }
    return Vector2::ZERO;
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

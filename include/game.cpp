#include <ctime>
#include "game.h"
#include "Arduino.h"
#include "vector2.h"
#include "panels.cpp"

const float TAU = 6.28318;

#define BUTTON_UP_PIN 6
#define BUTTON_DOWN_PIN 7

Game::Game() {
    std::srand(std::time(nullptr));

    for(Panel panel: PANELS){
        panel.index = panels.size();
        panel.physical_offset = Vector2(0, panels.size() * 32);
        panel.width = panel.rotation ? 32 : 64;
        panel.height = panel.rotation ? 64 : 32;
        panels.push_back(panel);
        
        std::list<Vector2> enemyPositions = getCharacterPositions('%', panel);
        for(Vector2 position : enemyPositions){
            Enemy enemy;
            enemy.position = position + panel.offset;
            enemy.spawnPosition = enemy.position;
            enemies.push_back(enemy);
        }
    }
    
    currentPanel = panels.front();
    renderer.renderPanel(currentPanel);

    player = Player();
    player.spawnPosition = getCharacterPosition('0', currentPanel);
    player.position = player.spawnPosition;
    movePlayerTo(player.position);

    lastEnemyMovedTime = millis();

    pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
    pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
}

void Game::start() {

    for(Panel panel : panels) {
        renderer.renderPanel(panel);
    }
}
void Game::restartFromLastCheckpoint(){
    
    renderer.drawPanelPixel(player.position - currentPanel.offset, currentPanel, renderer.pathColor);
    movePlayerTo(player.spawnPosition);
    player.direction = player.spawnDirection;

    for(Enemy &enemy: enemies) {
        Panel panel = getPanelAtPosition(enemy.position);
        renderer.drawPanelPixel(enemy.position - panel.offset, panel, renderer.pathColor);
        enemy.position = enemy.spawnPosition;
        enemy.activated = false;
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
}
std::list<Vector2> Game::getCharacterPositions(char character, Panel panel) {
    int x = 0;
    int y = 0;
    std::list<Vector2> positions;
    for(std::string row: panel.map){
        x = 0;
        for(char cell : row){
            if(cell == character){
                positions.push_back(Vector2(x,y));
            }
            x++;
        }
        y++;
    }
    return positions;
}

char Game::getCharacterAtPosition(Vector2 position, Panel panel) {
    return panel.map[position.y][position.x];
}
char Game::getCharacterAtPosition(Vector2 position) {
    Panel panel = getPanelAtPosition(position);
    return panel.getCharAtPosition(position - panel.offset);
}

void Game::movePlayer(Vector2 direction){
    player.lastPosition = player.position;
    movePlayerTo(player.position + direction);
}

void Game::movePlayerTo(Vector2 position) {
    
    char cell = getCharacterAtPosition(position);
    Serial.print("CELL: ");
    Serial.println(cell);
    switch(cell) {
        case '$':
            hasKey = true;
            break;
        case '#':
            if(!hasKey) return;
            hasKey = false;
            break;
        default: break;
    }

    player.position = position;
    if(player.position.x < currentPanel.offset.x || player.position.x >= currentPanel.offset.x + currentPanel.width ||
       player.position.y < currentPanel.offset.y || player.position.y >= currentPanel.offset.y + currentPanel.height) {
        currentPanel = getPanelAtPosition(player.position);
        player.spawnPosition = player.position;
        player.spawnDirection = player.direction;
        renderer.renderPanel(currentPanel);
    }

    player.availableDirections = getAvailableDirections(Vector2(player.position.x, player.position.y));
    if(player.availableDirections.size() == 1){
        player.direction = player.availableDirections.front();
    }

}

std::vector<Vector2> Game::getForwardDirections(Player p){
    std::vector<Vector2> directions;
    for(Vector2 dir : p.availableDirections) {
        if(dir != Vector2(-p.direction.x, -p.direction.y)){
            directions.push_back(dir);
        }
    }
    return directions;
}

std::vector<Vector2> Game::getBackwardDirections(Player p){
    std::vector<Vector2> directions;
    for(Vector2 dir : p.availableDirections) {
        if(dir != Vector2(p.direction.x, p.direction.y)){
            directions.push_back(dir);
        }
    }
    return directions;
}

void Game::moveForward(){
    player.forwardDirections = getForwardDirections(player);
    if(player.forwardDirections.size() == 1){
        player.direction = player.forwardDirections.front();
    }
    else {
        isPathSelection = true;
        pathSelectionTime = millis();
        return;
    }

    movePlayer(player.direction);

    player.forwardDirections = getForwardDirections(player);
    if(player.forwardDirections.size() != 1){
        isPathSelection = true;
        pathSelectionTime = millis();
    }
}

void Game::moveBackward(){
    player.forwardDirections = getBackwardDirections(player);
    Vector2 direction;
    if(player.forwardDirections.size() == 1){
        direction = player.forwardDirections.front();
        player.direction = Vector2(-direction.x, -direction.y);
    }
    else {
        isPathSelection = true;
        pathSelectionTime = millis();
        return;
    }
    movePlayer(direction);

    player.forwardDirections = getBackwardDirections(player);
    if(player.forwardDirections.size() != 1){
        isPathSelection = true;
        pathSelectionTime = millis();
    }
}

Panel Game::getPanelAtPosition(Vector2 position){
    for(Panel panel : panels) {
        if(position.x >= panel.offset.x && position.x < panel.offset.x + panel.width &&
           position.y >= panel.offset.y && position.y < panel.offset.y + panel.height) {
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
        char cell = getCharacterAtPosition(newPos);
        if(cell == '*' || cell == '$' || cell == '0') {
            directions.push_back(dir);
        }
        if(cell == '#' && hasKey) directions.push_back(dir);
    }
    return directions;
}
std::list<Vector2> Game::getAvailableEnemyDirections(Vector2 position) {
    std::list<Vector2> directions;
    std::list<Vector2> possibleDirections = {
        Vector2::UP,
        Vector2::DOWN,
        Vector2::LEFT,
        Vector2::RIGHT
    };
    for(Vector2 dir : possibleDirections) {
        Vector2 newPos = position + dir;
        char cell = getCharacterAtPosition(newPos);
        if(cell == '*') {
            directions.push_back(dir);
        }
    }
    return directions;
}


void Game::moveEnemy(Enemy &enemy){
    enemy.availableDirections = getAvailableEnemyDirections(enemy.position);
    enemy.forwardDirections = getForwardDirections(enemy);
    
    if(enemy.forwardDirections.size() <= 0){
        enemy.direction = Vector2::ZERO - enemy.direction;
    }
    else if(enemy.forwardDirections.size() == 1){
        enemy.direction = enemy.forwardDirections.front();
    }
    else {
        enemy.direction = enemy.forwardDirections[std::rand() % enemy.forwardDirections.size()];
    }

    enemy.position += enemy.direction;

}

void Game::update() {

    for(Enemy &enemy: enemies){
        if(player.position == enemy.position){
            Serial.println("GAME OVER");
            restartFromLastCheckpoint();
            return;
        }
    }

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
            player.position.x - currentPanel.offset.x,
            player.position.y - currentPanel.offset.y
        ),
        currentPanel,
        renderer.pathColor
    ); // erase player previous position

    renderer.drawPanelBorder(currentPanel, renderer.matrix.color565(0, 0, 0)); // Draw panel border in green
    
    if(isPathSelection){
        int pathIndex = int((millis() - pathSelectionTime) / pathSelectionDuration) % player.forwardDirections.size();
        for(Vector2 dir: player.forwardDirections){
            renderer.drawPanelPixel(
                player.position + dir - currentPanel.offset,
                currentPanel,
                renderer.pathColor
            );
        }
        if(isUpJustPressed){
            isPathSelection = false;
            player.direction = player.forwardDirections[pathIndex];
            movePlayer(player.direction);
        }
        // draw current path
        else {
            renderer.drawPanelPixel(
                Vector2(
                    player.position.x + player.forwardDirections[pathIndex].x - currentPanel.offset.x,
                    player.position.y + player.forwardDirections[pathIndex].y - currentPanel.offset.y
                ),
                currentPanel,
                renderer.pathSelectionColor
            );
        }
        
    }
    else {
        if(!digitalRead(BUTTON_UP_PIN)){
            // movePlayer(Vector2::RIGHT);
            moveForward();
        }
        if(!digitalRead(BUTTON_DOWN_PIN)){
            // movePlayer(Vector2::UP);
            moveBackward();
        }
    }

    renderer.drawPanelBorder(currentPanel, renderer.matrix.color565(32,32, 0)); // Draw panel border in green
    
    // Draw player as white pixel
    renderer.drawPanelPixel(
        player.position - currentPanel.offset,
        currentPanel,
        hasKey && int(millis() / 300.0) % 3 > 1 ? renderer.keyColor : renderer.playerColor
    );

    if(millis() - lastEnemyMovedTime > enemyMoveTime){
        lastEnemyMovedTime = millis();
        for(Enemy &enemy: enemies){
            Panel p = getPanelAtPosition(enemy.position);
            if(!enemy.activated && currentPanel.index != p.index) {
                renderer.drawPanelPixel(enemy.position - p.offset, p, renderer.enemyColor);
                continue;
            }
            renderer.drawPanelPixel(enemy.position - p.offset, p,  renderer.pathColor);
            enemy.activated = true;
            moveEnemy(enemy);
            p = getPanelAtPosition(enemy.position);
            renderer.drawPanelPixel(enemy.position - p.offset, p, renderer.enemyColor);
        }
    }

    renderer.update();
}

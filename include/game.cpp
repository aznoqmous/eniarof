#include "game.h"
#include "Arduino.h"

const float TAU = 6.28318;

#define BUTTON_UP_PIN 6
#define BUTTON_DOWN_PIN 7

Game::Game() {
    player = Player();

    panels.push_back(Panel(64, 32, 0, 0, 0, 0, 0));
    panels.push_back(Panel(32, 64, 0, 32, 0, 32, PI/2.0));

    currentPanel = panels.front();

    pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
    pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
}

void Game::start() {
    // movePlayer(30, 30);
    movePlayer(30, 30);
}

void Game::movePlayer(int x, int y){
    movePlayerTo(player.x + x, player.y + y);
}

void Game::movePlayerTo(int x, int y) {
    player.x = x;
    player.y = y;
    Serial.println("Player moved to: ");
    Serial.println(player.x);
    Serial.println(player.y);
    if(player.x < currentPanel.offset_x || player.x >= currentPanel.offset_x + currentPanel.width ||
       player.y < currentPanel.offset_y || player.y >= currentPanel.offset_y + currentPanel.height) {
        currentPanel = getPanelAtPosition(player.x, player.y);
    }
}

Panel Game::getPanelAtPosition(int x, int y){
    for(Panel panel : panels) {
        if(x >= panel.offset_x && x < panel.offset_x + panel.width &&
           y >= panel.offset_y && y < panel.offset_y + panel.height) {
            return panel;
        }
    }
    return currentPanel; // Return current panel if no other found
}

void Game::update() {
    // movePlayer(player.x + 1, player.y + 1);
    
    renderer.clearPanel(currentPanel);
    
    if(digitalRead(BUTTON_UP_PIN)){
        movePlayer(0, 1);
    }
    if(digitalRead(BUTTON_DOWN_PIN)){
        movePlayer(0, -1);
    }

    renderer.drawPanelBorder(currentPanel, 0x00FF00); // Draw panel border in green
    
    renderer.drawPanelPixel(player.x, player.y, currentPanel, 0xFFFF); // Draw player as white pixel
    
    renderer.update();
}

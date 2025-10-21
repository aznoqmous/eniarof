#pragma once
#include "player.cpp"
#include "panel.cpp"
#include "list"
#include "renderer.cpp"

class Game {
public:
    Game();
    void start();
    void update();
    void movePlayer(int dx, int dy);
    void movePlayerTo(int x, int y);
    Panel getPanelAtPosition(int x, int y);
private:
    Player player;
    std::list<Panel> panels;
    Renderer renderer;
    Panel currentPanel;
};
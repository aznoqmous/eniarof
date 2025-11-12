#pragma once
#include <Adafruit_Protomatter.h>
#include "panel.h"

class Renderer {
    public:
        Renderer();
        void update();
        void clear();
        void clearPanel(Panel panel);
        void drawPanelPixel(Vector2 position, Panel panel, uint16_t color);
        void drawPanelBorder(Panel panel, uint16_t color);
        void drawPixel(Vector2 position, uint16_t color);
        void renderPanel(Panel panel);
        void renderPortal(Vector2 position, Panel panel);
        void renderEnemy(Vector2 position, Panel panel);
        void renderArray(std::list<int> chars, int width, Vector2 position, Panel panel, u_int16_t color);

        Vector2 getTransformedPosition(Vector2 position, Panel panel);

        Adafruit_Protomatter matrix;

        u_int16_t pathColor;
        u_int16_t playerColor;
        u_int16_t keyColor;
        u_int16_t enemyColor;
        u_int16_t pathSelectionColor;
        u_int16_t tpColor;
        
        u_int16_t white;

};
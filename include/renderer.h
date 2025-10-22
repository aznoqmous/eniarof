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
        Adafruit_Protomatter matrix;

};
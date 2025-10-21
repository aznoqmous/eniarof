#pragma once
#include <Adafruit_Protomatter.h>
#include "panel.h"

class Renderer {
    public:
        Renderer();
        void update();
        void clear();
        void clearPanel(Panel panel);
        void drawPanelPixel(int x, int y, Panel panel, uint16_t color);
        void drawPanelBorder(Panel panel, uint16_t color);
        void drawPixel(int x, int y, uint16_t color);
    private:
        Adafruit_Protomatter matrix;
};
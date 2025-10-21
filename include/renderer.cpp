#include "renderer.h"

uint8_t rgbPins[]  = {42, 41, 40, 38, 39, 37};
uint8_t addrPins[] = {45, 36, 48, 35, 21};
uint8_t clockPin   = 2;
uint8_t latchPin   = 47;
uint8_t oePin      = 14;

// uint16_t PATH_COLOR = matrix.color565(4, 4, 4);

Renderer::Renderer() : matrix(64,          // Width of matrix (or matrices, if tiled horizontally)
           6,           // Bit depth, 1-6
           1, rgbPins,  // # of matrix chains, array of 6 RGB pins for each
           4, addrPins, // # of address pins (height is inferred), array of pins
           clockPin, latchPin, oePin, // Other matrix control pins
           false,       // No double-buffering here (see "doublebuffer" example)
           -2)          // Row tiling: two rows in "serpentine" path
{
    ProtomatterStatus status = matrix.begin();
    Serial.print("Protomatter begin() status: ");
    Serial.println((int)status);
    if(status != PROTOMATTER_OK) {
        // DO NOT CONTINUE if matrix setup encountered an error.
        for(;;);
    }
}

void Renderer::clear(){
    matrix.fillRect(0, 0, 64, 32, matrix.color565(0, 0, 0));
    
}

void Renderer::update() {
    matrix.show();
}

void Renderer::clearPanel(Panel panel) {
    matrix.fillRect(
        panel.physical_offset_x,
        panel.physical_offset_y,
        64,
        32,
        matrix.color565(0, 0, 0)
    );
}

void Renderer::drawPanelPixel(int x, int y, Panel panel, uint16_t color) {
    float rotation = -panel.rotation;
    float local_x = x - panel.offset_x;
    float local_y = y - panel.offset_y;
    
    float final_x = float(local_x) * cos(rotation) - float(local_y) * sin(rotation);
    float final_y = float(local_x) * sin(rotation) + float(local_y) * cos(rotation);

    if(panel.rotation > 0){
        final_y += 32.0;
    }

    matrix.drawPixel(panel.physical_offset_x + final_x, panel.physical_offset_y + final_y, color);
}

void Renderer::drawPixel(int x, int y, uint16_t color) {
    matrix.drawPixel(x, y, color);
}

void Renderer::drawPanelBorder(Panel panel, uint16_t color) {
    matrix.drawRect(
        panel.physical_offset_x,
        panel.physical_offset_y,
        64,
        32,
        color
    );
}
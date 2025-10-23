#include "renderer.h"
#include "panels.cpp"

uint8_t rgbPins[]  = {42, 41, 40, 38, 39, 37};
uint8_t addrPins[] = {45, 36, 48, 35, 21};
uint8_t clockPin   = 2;
uint8_t latchPin   = 47;
uint8_t oePin      = 14;

Renderer::Renderer() : matrix(
        64,          // Width of matrix (or matrices, if tiled horizontally)
        3,           // Bit depth, 1-6
        1, rgbPins,  // # of matrix chains, array of 6 RGB pins for each
        4, addrPins, // # of address pins (height is inferred), array of pins
        clockPin, latchPin, oePin, // Other matrix control pins
        true,       // No double-buffering here (see "doublebuffer" example)
        14 // Row tiling
    )
{
    ProtomatterStatus status = matrix.begin();
    Serial.print("Protomatter begin() status: ");
    Serial.println((int)status);
    if(status != PROTOMATTER_OK) {
        for(;;);
    }

    playerColor = matrix.color565(255,255,255);
    pathColor = matrix.color565(0, 32, 64);
    keyColor = matrix.color565(255,131,0);
    enemyColor = matrix.color565(255, 0, 0);
    pathSelectionColor = matrix.color565(0, 255, 0);
}

void Renderer::clear(){
    matrix.fillRect(0, 0, 64, 32, matrix.color565(0, 0, 0));
}

void Renderer::update() {
    matrix.show();
}

void Renderer::clearPanel(Panel panel) {
    matrix.fillRect(
        panel.physical_offset.x,
        panel.physical_offset.y,
        64,
        32,
        matrix.color565(0, 0, 0)
    );
}

void Renderer::drawPanelPixel(Vector2 localPos, Panel panel, uint16_t color) {
    float rotation = -panel.rotation;
    
    float final_x = float(localPos.x) * cos(rotation) - float(localPos.y) * sin(rotation);
    float final_y = float(localPos.x) * sin(rotation) + float(localPos.y) * cos(rotation);

    if(panel.rotation > 0){
        final_y += 32.0;
    }

    matrix.drawPixel(panel.physical_offset.x + final_x, panel.physical_offset.y + final_y, color);
}

void Renderer::renderPanel(Panel panel){
    int x = 0;
    int y = 0;
    for(std::string row: panel.map){
        x = 0;
        for(char cell : row){
            switch (cell)
            {
                case '*':
                case '=':
                    if(panel.rotation != 0){
                        drawPanelPixel(
                            Vector2(0, panel.height) + Vector2(y, -x),
                            panel,
                            pathColor
                        );
                    }
                    else {
                        drawPanelPixel(
                            Vector2(x, y),
                            panel,
                            pathColor
                        );
                    }
                break;
                case '+':
                case '#':
                case '$':
                    if(panel.rotation != 0){
                        drawPanelPixel(
                            Vector2(0, panel.height) + Vector2(y, -x),
                            panel,
                            keyColor
                        );
                    }
                    else {
                        drawPanelPixel(
                            Vector2(x, y),
                            panel,
                            keyColor
                        );
                    }
                break;                
                default:
                break;
            }
            x++;
        }
        y++;
    }
}

void Renderer::drawPixel(Vector2 position, uint16_t color) {
    matrix.drawPixel(position.x, position.y, color);
}

void Renderer::drawPanelBorder(Panel panel, uint16_t color) {
    matrix.drawRect(
        panel.physical_offset.x,
        panel.physical_offset.y,
        64,
        32,
        color
    );
}
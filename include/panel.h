#pragma once
#include <map>
#include <string>
#include "vector2.h"
#include "panels.cpp"

const int PANEL_WIDTH = 64;
const int PANEL_HEIGHT = 32;

class Panel {
public:
    Panel();
    Panel(
        int offset_x,
        int offset_y, 
        int physical_offset_x,
        int physical_offset_y,
        float rotation,
        int index
    ) : 
    offset_x(offset_x),
    offset_y(offset_y),
    physical_offset_x(physical_offset_x),
    physical_offset_y(physical_offset_y),
    rotation(rotation),
    index(index)
    {
        width = rotation != 0 ? PANEL_HEIGHT : PANEL_WIDTH;
        height = rotation != 0 ? PANEL_WIDTH : PANEL_HEIGHT;
    }
    int width;
    int height;
    int offset_x = 0;
    int offset_y = 0;
    int physical_offset_x = 0;
    int physical_offset_y = 0;
    float rotation = 0;
    int index = 0;

    char getCharAtPosition(Vector2 position);

};
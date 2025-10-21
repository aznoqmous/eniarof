#pragma once

class Panel {
public:
    Panel();
    Panel(
        int width, 
        int height,
        int offset_x,
        int offset_y, 
        int physical_offset_x,
        int physical_offset_y,
        float rotation
    ) : 
    width(width),
    height(height),
    offset_x(offset_x),
    offset_y(offset_y),
    physical_offset_x(physical_offset_x),
    physical_offset_y(physical_offset_y),
    rotation(rotation)
    {}
    int width;
    int height;
    int offset_x = 0;
    int offset_y = 0;
    int physical_offset_x = 0;
    int physical_offset_y = 0;
    float rotation = 0;
};
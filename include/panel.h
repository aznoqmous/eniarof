#pragma once
#include <map>
#include <string>
#include <vector>
#include "vector2.h"

class Panel {
public:
    Panel();
    Panel(
        int offset_x,
        int offset_y, 
        float rotation,
        std::vector<std::string>map
    ) : 
    offset_x(offset_x),
    offset_y(offset_y),
    rotation(rotation),
    map(std::move(map))
    {
    }

    int offset_x = 0;
    int offset_y = 0;
    float rotation = 0;
    std::vector<std::string> map;

    int width;
    int height;
    int physical_offset_x = 0;
    int physical_offset_y = 0;
    int index = 0;

    char getCharAtPosition(Vector2 position);

};
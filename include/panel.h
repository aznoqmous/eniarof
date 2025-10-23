#pragma once
#include <map>
#include <string>
#include <vector>
#include "vector2.h"

class Panel {
public:
    Panel();
    Panel(
        Vector2 offset,
        float rotation,
        std::vector<std::string>map
    ) : 
    offset(offset),
    rotation(rotation),
    map(std::move(map))
    {
    }

    Vector2 offset;
    float rotation = 0;
    std::vector<std::string> map;

    int width = 64;
    int height = 32;
    Vector2 physical_offset;
    int index = 0;

    char getCharAtPosition(Vector2 position);
    void setCharAtPosition(Vector2 position, char value);
};
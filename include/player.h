#pragma once
#include "vector2.h"
#include <list>
#include <vector>

class Player {
public:
    Vector2 position;
    Vector2 spawnPosition;
    Vector2 spawnDirection;
    Vector2 lastPosition;
    Vector2 direction;
    std::list<Vector2> availableDirections;
    std::vector<Vector2> forwardDirections;
};
#pragma once
#include "vector2.h"
#include "player.h"

class Enemy : public Player {
    public:
        bool activated = false;
};
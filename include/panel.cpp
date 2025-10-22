#include "panel.h"
#include "panels.cpp"

Panel::Panel(){};
char Panel::getCharAtPosition(Vector2 position){
    return MAPS[index][position.y][position.x];
}

#include "panel.h"
#include "panels.cpp"

Panel::Panel(){};
char Panel::getCharAtPosition(Vector2 position){
    if(rotation) return map[position.x][height - position.y];
    return map[position.y][position.x];
}
void Panel::setCharAtPosition(Vector2 position, char value){
    if(rotation) map[position.x][height - position.y] = value;
    else map[position.y][position.x] = value;
}
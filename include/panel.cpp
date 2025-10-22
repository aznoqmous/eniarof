#include "panel.h"

Panel::Panel(){};
char Panel::getCharAtPosition(Vector2 position){
    return map[position.y][position.x];
}

#include "panel.h"
#include "panels.cpp"

Panel::Panel(){};

void Panel::loadPaths(){
    int x = 0;
    int y = 0;
    for(std::string row: map){
        x = 0;
        for(char cell : row){
            if(cell == '*'){
                paths.push_back(Vector2(x, y));
            }
            x++;
        }
        y++;
    }
}

char Panel::getCharAtPosition(Vector2 position){
    if(rotation) return map[position.x][height - position.y];
    return map[position.y][position.x];
}
void Panel::setCharAtPosition(Vector2 position, char value){
    if(rotation) map[position.x][height - position.y] = value;
    else map[position.y][position.x] = value;
}
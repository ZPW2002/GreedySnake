#include "snake.h"

Snake::Snake()
{
    x = y = dir = 0;
}

void Snake::set(int x, int y, int dir)
{
    this->x=x;
    this->y=y;
    this->dir=dir;
}

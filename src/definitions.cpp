#include "definitions.h"

BoardLocation::BoardLocation(const int _x, const int _y)
{
    x = _x;
    y = _y;
}

bool operator<(const BoardLocation& left, const BoardLocation& right)
{
    if(left.x < right.x || left.x == right.x && left.y < right.y)
    {
        return true;
    }
    else
    {
        return false;
    }
}

#include "definitions.h"

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

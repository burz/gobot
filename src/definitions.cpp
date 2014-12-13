#include "definitions.h"

namespace {
const char emptyShorter[] = "E";
const char blackShorter[] = "B";
const char whiteShorter[] = "W";
const char empty[] = "EMPTY";
const char black[] = "BLACK";
const char white[] = "WHITE";
const char blackShorterColored[] = "\e[34mB\e[0m";
const char whiteShorterColored[] = "\e[93mW\e[0m";
} // close anonymous namespace

const char* spaceStateString(
        const SpaceState state,
        const bool shorter,
        const bool colored)
{
    if(colored)
    {
        if(state == EMPTY)
        {
            return emptyShorter;
        }
        else if(state == BLACK)
        {
            return blackShorterColored;
        }
        else
        {
            return whiteShorterColored;
        }
    }
    else if(shorter)
    {
        if(state == EMPTY)
        {
            return emptyShorter;
        }
        else if(state == BLACK)
        {
            return blackShorter;
        }
        else
        {
            return whiteShorter;
        }
    }
    else
    {
        if(state == EMPTY)
        {
            return empty;
        }
        else if(state == BLACK)
        {
            return black;
        }
        else
        {
            return white;
        }
    }
}

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

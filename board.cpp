#include "board.h"

#include <stdio.h>

namespace {
const char empty[] = "EMPTY";
const char black[] = "BLACK";
const char white[] = "WHITE";
} // close anonymous namespace

const char* const spaceStateString(const SpaceState state)
{
    if(state == EMPTY) {
        return empty;
    }
    else if(state == BLACK) {
        return black;
    }
    else {
        return white;
    }
}

Space::Space(void)
{
    state = EMPTY;
}

SpaceState Space::getState(void) const
{
    return state;
}

void Space::changeState(const SpaceState _state)
{
    state = _state;
}

Board::Board(const int _size)
    : size(_size)
{
    spaces = new Space*[size]();

    for(int i = 0; i < size; ++i)
    {
        spaces[i] = new Space[size]();
    }
}

Board::~Board(void)
{
    for(int i = 0; i < size; ++i)
    {
        delete[] spaces[i];
    }

    delete[] spaces;
}

int Board::getSize(void) const
{
    return size;
}

SpaceState Board::getState(const int x, const int y) const
{
    return spaces[x][y].getState();
}

void Board::changeState(const int x, const int y, const SpaceState state)
{
    spaces[x][y].changeState(state);
}

void Board::print(void) const
{
    for(int x = 0; x < size; ++x) {
        for(int y = 0; y < size; ++y) {
            printf("%s, ", spaceStateString(getState(x, y)));
        } 

        printf("\b\b \n");
    }
}

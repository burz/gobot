#include "board.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

Board::Board(const int _size)
    : size(_size)
{
    spaces = new Space*[size]();

    Block* block = new Block();

    blocks.push_back(block);

    Space space(block);

    for(int i = 0; i < size; ++i)
    {
        spaces[i] = (Space*) malloc(sizeof(Space) * size);

        for(int j = 0; j < size; ++j)
        {
            spaces[i][j] = space;

            BoardLocation location;

            location.x = i;
            location.y = j;

            block->add(location, 0);
        }

        assert(spaces[i] != 0);
    }
}

Board::~Board(void)
{
    for(int i = 0; i < size; ++i)
    {
        free(spaces[i]);
    }

    std::vector<Block*>::iterator itt = blocks.begin();

    for( ; itt != blocks.end(); ++itt)
    {
        delete *itt;
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

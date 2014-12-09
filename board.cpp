#include "board.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <set>

Board::Board(const int _size)
    : size(_size)
{
    spaces = new Space*[size]();

    Block* block = new Block();

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

    std::set<Block*> blocks;

    for(int x = 0; x < size; ++x) {
        for(int y = 0; y < size; ++y) {
            blocks.insert(spaces[x][y].getBlock());
        }
    }

    std::set<Block*>::iterator itt = blocks.begin();

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

void Board::playMove(const int x, const int y, const SpaceState state)
{
    BoardLocation location;

    location.x = x;
    location.y = y;

    int libertiesGained = 0;

    Block* block0 = getBlock(x, y);
    Block* block1 = getBlock(x - 1, y - 1);
    Block* block2 = getBlock(x - 1, y + 1);
    Block* block3 = getBlock(x + 1, y - 1);
    Block* block4 = getBlock(x + 1, y + 1);

    assert(block0);

    block0->removeLocation(location);

    Block* currentBlock = new Block(state, location, 0);

    if(block1)
    {
        if(block1->getState() == state && block1 != currentBlock)
        {
            block1->absorb(currentBlock);

            delete currentBlock;

            currentBlock = block1;
        }
        else if(block1->getState() == EMPTY)
        {
            libertiesGained += 1;
        }
    }
    else if(block2)
    {
        if(block2->getState() == state && block2 != currentBlock)
        {
            block2->absorb(currentBlock);

            delete currentBlock;

            currentBlock = block2;
        }
        else if(block2->getState() == EMPTY)
        {
            libertiesGained += 1;
        }
    }
    else if(block3)
    {
        if(block3->getState() == state && block3 != currentBlock)
        {
            block3->absorb(currentBlock);

            delete currentBlock;

            currentBlock = block3;
        }
        else if(block3->getState() == EMPTY)
        {
            libertiesGained += 1;
        }
    }
    else if(block4)
    {
        if(block4->getState() == state && block4 != currentBlock)
        {
            block4->absorb(currentBlock);

            delete currentBlock;

            currentBlock = block4;
        }
        else if(block4->getState() == EMPTY)
        {
            libertiesGained += 1;
        }
    }

    currentBlock->setLiberties(currentBlock->getLiberties() + libertiesGained);
}

Block* Board::getBlock(const int x, const int y) const
{
    if(x < 0 || x >= size || y < 0 || y >= size)
    {
        return 0;
    }

    return spaces[x][y].getBlock();
}

void Board::changeBlocks(Block* from, Block* to)
{
    for(int x = 0; x < size; ++x) {
        for(int y = 0; y < size; ++y) {
            spaces[x][y].changeBlock(from, to);
        }
    }
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

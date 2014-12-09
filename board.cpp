#include "board.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <set>

int Board::updateAdjacentBlock(Block** currentBlock, Block* targetBlock)
{
    if(targetBlock->getState() == EMPTY)
    {
        return 1;
    }
    else if(targetBlock->getState() != (*currentBlock)->getState())
    {
        targetBlock->changeLiberties(-1);

        if(targetBlock->getLiberties() == 0)
        {
            if(targetBlock->getState() == BLACK)
            {
                score -= targetBlock->getSize();
            }
            else
            {
                score += targetBlock->getSize();
            }

            targetBlock->setState(EMPTY);
        }
    }
    else if(targetBlock != *currentBlock)
    {
        targetBlock->absorb(*currentBlock);

        changeBlocks(*currentBlock, targetBlock);

        delete *currentBlock;

        *currentBlock = targetBlock;
    }

    return 0;
}

Board::Board(const int _size, const float komi)
    : size(_size)
{
    score = komi;

    spaces = new Space*[size]();

    Block* block = new Block();

    Space space(block);

    for(int i = 0; i < size; ++i)
    {
        spaces[i] = (Space*) malloc(sizeof(Space) * size);

        assert(spaces[i] != 0);

        for(int j = 0; j < size; ++j)
        {
            spaces[i][j] = space;

            BoardLocation location;

            location.x = i;
            location.y = j;

            block->add(location, 0);
        }
    }
}

Board::~Board(void)
{
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

    for(int i = 0; i < size; ++i)
    {
        free(spaces[i]);
    }

    delete[] spaces;
}

int Board::getSize(void) const
{
    return size;
}

float Board::getScore(void) const
{
    return score;
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
    Block* block1 = getBlock(x - 1, y);
    Block* block2 = getBlock(x, y - 1);
    Block* block3 = getBlock(x + 1, y);
    Block* block4 = getBlock(x, y + 1);

    assert(block0);

    block0->removeLocation(location);

    Block* currentBlock = new Block(state, location, 0);

    if(block1)
    {
        libertiesGained += updateAdjacentBlock(&currentBlock, block1);
    }
    if(block2)
    {
        libertiesGained += updateAdjacentBlock(&currentBlock, block2);
    }
    if(block3)
    {
        libertiesGained += updateAdjacentBlock(&currentBlock, block3);
    }
    if(block4)
    {
        libertiesGained += updateAdjacentBlock(&currentBlock, block4);
    }

    currentBlock->changeLiberties(libertiesGained);

    setBlock(x, y, currentBlock);
}

Block* Board::getBlock(const int x, const int y) const
{
    if(x < 0 || x >= size || y < 0 || y >= size)
    {
        return 0;
    }

    return spaces[x][y].getBlock();
}

void Board::setBlock(const int x, const int y, Block* block)
{
    spaces[x][y].setBlock(block);
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

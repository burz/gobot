#include "board.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <set>

void Board::removeDeadBlock(Block* block)
{
    if(block->getState() == BLACK)
    {
        score -= block->getSize();
    }
    else
    {
        score += block->getSize();
    }

    block->setState(EMPTY);

    std::set<BoardLocation>::const_iterator itt = block->locationsBegin();
    std::set<BoardLocation>::const_iterator end = block->locationsEnd();

    for( ; itt != end; ++itt)
    {
        std::set<Block*> adjacentBlocks;

        adjacentBlocks.insert(getBlock(itt->x - 1, itt->y));
        adjacentBlocks.insert(getBlock(itt->x, itt->y - 1));
        adjacentBlocks.insert(getBlock(itt->x + 1, itt->y));
        adjacentBlocks.insert(getBlock(itt->x, itt->y + 1));

        std::set<Block*>::iterator blockItt = adjacentBlocks.begin();
        std::set<Block*>::iterator blockEnd = adjacentBlocks.end();

        for( ; blockItt != blockEnd; ++blockItt)
        {
            (*blockItt)->changeLiberties(1);
        }
    }
}

void Board::deleteAdjacentBlocks(
        Block* deleted,
        Block** block1,
        Block** block2,
        Block** block3,
        Block** block4)
{
    if(*block1 == deleted)
    {
        *block1 = 0;
    }
    if(*block2 == deleted)
    {
        *block2 = 0;
    }
    if(*block3 == deleted)
    {
        *block3 = 0;
    }
    if(*block4 == deleted)
    {
        *block4 = 0;
    }
}

int Board::checkIfEncountered(
        std::set<BoardLocation> *encounteredLocations,
        const int x,
        const int y)
{
    BoardLocation location;

    location.x = x;
    location.y = y;

    if(encounteredLocations->find(location) == encounteredLocations->end())
    {
        encounteredLocations->insert(location);

        return 1;
    }

    return 0;
}

int Board::adjacentLiberties(Block* block0, Block* block)
{
    int result = 0;

    std::set<BoardLocation> encounteredLocations;

    std::set<BoardLocation>::iterator itt = block0->locationsBegin();
    std::set<BoardLocation>::iterator end = block0->locationsEnd();

    for( ; itt != end; ++itt)
    {
        Block* block1 = getBlock(itt->x - 1, itt->y);
        Block* block2 = getBlock(itt->x, itt->y - 1);
        Block* block3 = getBlock(itt->x + 1, itt->y);
        Block* block4 = getBlock(itt->x, itt->y + 1);

        if(block1 && block1->getState() == EMPTY && block->touches(itt->x - 1, itt->y))
        {
            result += checkIfEncountered(&encounteredLocations, itt->x - 1, itt->y);
        }
        else if(block2 && block2->getState() == EMPTY && block->touches(itt->x, itt->y - 1))
        {
            result += checkIfEncountered(&encounteredLocations, itt->x, itt->y - 1);
        }
        else if(block3 && block3->getState() == EMPTY && block->touches(itt->x + 1, itt->y))
        {
            result += checkIfEncountered(&encounteredLocations, itt->x + 1, itt->y);
        }
        else if(block4 && block4->getState() == EMPTY && block->touches(itt->x, itt->y + 1))
        {
            result += checkIfEncountered(&encounteredLocations, itt->x, itt->y + 1);
        }

        printf("Location: (%d, %d), Result: %d\n", itt->x, itt->y, result);
    }

    return result;
}

void Board::absorbAdjacentBlock(Block** currentBlock, Block* targetBlock, Block** deleted)
{
    int adjacentLibs = adjacentLiberties(*currentBlock, targetBlock);

    targetBlock->absorb(*currentBlock);

    changeBlocks(*currentBlock, targetBlock);

    delete *currentBlock;

    *deleted = *currentBlock;

    *currentBlock = targetBlock;

    (*currentBlock)->changeLiberties(adjacentLibs ? -adjacentLibs : -1);
}

Block* Board::updateAdjacentBlock(Block** currentBlock, Block* targetBlock)
{
    Block* deleted = 0;

    if(targetBlock->getState() != EMPTY)
    {
        if(targetBlock->getState() != (*currentBlock)->getState())
        {
            targetBlock->changeLiberties(-1);

            if(targetBlock->getLiberties() == 0)
            {
                removeDeadBlock(targetBlock);
            }
        }
        else if(targetBlock != *currentBlock)
        {
            absorbAdjacentBlock(currentBlock, targetBlock, &deleted);
        }
    }

    return deleted;
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

    Block* currentBlock = new Block(state);

    Block* deleted = 0;

    if(block1)
    {
        deleted = updateAdjacentBlock(&currentBlock, block1);
        deleteAdjacentBlocks(deleted, &block1, &block2, &block3, &block4);
    }
    if(block2)
    {
        deleted = updateAdjacentBlock(&currentBlock, block2);
        deleteAdjacentBlocks(deleted, &block1, &block2, &block3, &block4);
    }
    if(block3)
    {
        deleted = updateAdjacentBlock(&currentBlock, block3);
        deleteAdjacentBlocks(deleted, &block1, &block2, &block3, &block4);
    }
    if(block4)
    {
        deleted = updateAdjacentBlock(&currentBlock, block4);
        deleteAdjacentBlocks(deleted, &block1, &block2, &block3, &block4);
    }

    if(block1 && block1->getState() == EMPTY && !currentBlock->touches(x - 1, y))
    {
        libertiesGained += 1;
    }
    if(block2 && block2->getState() == EMPTY && !currentBlock->touches(x, y - 1))
    {
        libertiesGained += 1;
    }
    if(block3 && block3->getState() == EMPTY && !currentBlock->touches(x + 1, y))
    {
        libertiesGained += 1;
    }
    if(block4 && block4->getState() == EMPTY && !currentBlock->touches(x, y + 1))
    {
        libertiesGained += 1;
    }

    currentBlock->add(location, libertiesGained);

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

#include "board.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <set>

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

int canCount(std::set<BoardLocation>* counted, const int x, const int y)
{
    BoardLocation location;

    location.x = x;
    location.y = y;

    if(counted->find(location) == counted->end())
    {
        counted->insert(location);

        return 1;
    }

    return 0;
}

void Board::recalculateLiberties(Block* block)
{
    if(block->getState() == EMPTY)
    {
        return;
    }

    int result = 0;
    std::set<BoardLocation> counted;

    std::set<BoardLocation>::const_iterator itt = block->locationsBegin();
    std::set<BoardLocation>::const_iterator end = block->locationsEnd();

    for( ; itt != end; ++itt)
    {
        Block* block1 = getBlock(itt->x - 1, itt->y);
        Block* block2 = getBlock(itt->x, itt->y - 1);
        Block* block3 = getBlock(itt->x + 1, itt->y);
        Block* block4 = getBlock(itt->x, itt->y + 1);

        if(block1 && block1->getState() == EMPTY)
        {
            result += canCount(&counted, itt->x - 1, itt->y);
        }
        if(block2 && block2->getState() == EMPTY)
        {
            result += canCount(&counted, itt->x, itt->y - 1);
        }
        if(block3 && block3->getState() == EMPTY)
        {
            result += canCount(&counted, itt->x + 1, itt->y);
        }
        if(block4 && block4->getState() == EMPTY)
        {
            result += canCount(&counted, itt->x, itt->y + 1);
        }
    }

    block->setLiberties(result);
}

bool Board::handleAdjacentBlock(Block* currentBlock, Block* block)
{
    if(block->getState() == currentBlock->getState())
    {
        currentBlock->absorb(block);

        changeBlocks(block, currentBlock);

        delete block;

        return true;
    }

    return false;
}

void Board::removeDeadGroup(Block* block)
{
    std::set<BoardLocation>::const_iterator itt = block->locationsBegin();
    std::set<BoardLocation>::const_iterator end = block->locationsEnd();

    std::set<Block*> adjacentBlocks;

    for( ; itt != end; ++itt)
    {
        Block* block1 = getBlock(itt->x - 1, itt->y);
        Block* block2 = getBlock(itt->x, itt->y - 1);
        Block* block3 = getBlock(itt->x + 1, itt->y);
        Block* block4 = getBlock(itt->x, itt->y + 1);

        if(block1)
        {
            adjacentBlocks.insert(block1);
        }
        if(block2)
        {
            adjacentBlocks.insert(block2);
        }
        if(block3)
        {
            adjacentBlocks.insert(block3);
        }
        if(block4)
        {
            adjacentBlocks.insert(block4);
        }
    }

    block->setState(EMPTY);

    std::set<Block*>::iterator adjItt = adjacentBlocks.begin();
    std::set<Block*>::iterator adjEnd = adjacentBlocks.end();

    for( ; adjItt != adjEnd; ++adjItt)
    {
        recalculateLiberties(*adjItt);
    }
}

void Board::handlePossiblyDeadBlocks(
        Block* block1,
        Block* block2,
        Block* block3,
        Block* block4)
{
    if(block1)
    {
        recalculateLiberties(block1);

        if(block1->getLiberties() == 0)
        {
            removeDeadGroup(block1);
        }
    }
    if(block2)
    {
        recalculateLiberties(block2);

        if(block2->getLiberties() == 0)
        {
            removeDeadGroup(block2);
        }
    }
    if(block3)
    {
        recalculateLiberties(block3);

        if(block3->getLiberties() == 0)
        {
            removeDeadGroup(block3);
        }
    }
    if(block4)
    {
        recalculateLiberties(block4);

        if(block4->getLiberties() == 0)
        {
            removeDeadGroup(block4);
        }
    }
}

void fixBlockPointers(
        Block* block0,
        Block** block1,
        Block** block2,
        Block** block3,
        Block** block4)
{
    if(*block1 == block0)
    {
        *block1 = 0;
    }
    if(*block2 == block0)
    {
        *block2 = 0;
    }
    if(*block3 == block0)
    {
        *block3 = 0;
    }
    if(*block4 == block0)
    {
        *block4 = 0;
    }
}

void Board::playMove(const int x, const int y, const SpaceState state)
{
    BoardLocation location;

    location.x = x;
    location.y = y;

    Block* block0 = getBlock(x, y);
    Block* block1 = getBlock(x - 1, y);
    Block* block2 = getBlock(x, y - 1);
    Block* block3 = getBlock(x + 1, y);
    Block* block4 = getBlock(x, y + 1);

    assert(block0);

    block0->removeLocation(location);

    Block* currentBlock = new Block(state, location, 0);

    setBlock(x, y, currentBlock);

    if(block1 && handleAdjacentBlock(currentBlock, block1))
    {
        fixBlockPointers(block1, &block1, &block2, &block3, &block4);
    }
    if(block2 && handleAdjacentBlock(currentBlock, block2))
    {
        fixBlockPointers(block2, &block1, &block2, &block3, &block4);
    }
    if(block3 && handleAdjacentBlock(currentBlock, block3))
    {
        fixBlockPointers(block3, &block1, &block2, &block3, &block4);
    }
    if(block4 && handleAdjacentBlock(currentBlock, block4))
    {
        fixBlockPointers(block4, &block1, &block2, &block3, &block4);
    }

    recalculateLiberties(currentBlock);

    handlePossiblyDeadBlocks(block1, block2, block3, block4);
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

void Board::calculatePerimeterFeaturesForBlock(
        PerimeterFeatureState* state,
        Block* block0,
        Block* block,
        const BoardLocation location) const
{
    if(block0 != block)
    {
        state->perimeter.insert(location);
    }
    if(block->getState() == EMPTY)
    {
        state->liberties.insert(location);
    }
    else if(block0->getState() != block->getState())
    {
        state->opponents.insert(location);
        state->adjacentOpponentBlocks.insert(block);
    }
}

void Board::generatePerimeterFeatures(BlockFinalFeatures *features, Block* block) const
{
    PerimeterFeatureState state;

    std::set<BoardLocation>::const_iterator itt = block->locationsBegin();
    std::set<BoardLocation>::const_iterator end = block->locationsEnd();

    int minX = itt->x;
    int maxX = itt->x;
    int minY = itt->y;
    int maxY = itt->y;

    for( ; itt != end; ++itt)
    {
        if(itt->x < minX)
        {
            minX = itt->x;
        }
        else if(itt->x > maxX)
        {
            maxX = itt->x;
        }
        if(itt->y < minY)
        {
            minY = itt->y;
        }
        else if(itt->y > maxY)
        {
            maxY = itt->y;
        }

        Block* block1 = getBlock(itt->x - 1, itt->y);
        Block* block2 = getBlock(itt->x, itt->y - 1);
        Block* block3 = getBlock(itt->x + 1, itt->y);
        Block* block4 = getBlock(itt->x, itt->y + 1);

        if(block1)
        {
            BoardLocation location;

            location.x = itt->x - 1;
            location.y = itt->y;

            calculatePerimeterFeaturesForBlock(&state, block, block1, location);
        }
        if(block2)
        {
            BoardLocation location;

            location.x = itt->x;
            location.y = itt->y - 1;

            calculatePerimeterFeaturesForBlock(&state, block, block2, location);
        }
        if(block3)
        {
            BoardLocation location;

            location.x = itt->x + 1;
            location.y = itt->y;

            calculatePerimeterFeaturesForBlock(&state, block, block3, location);
        }
        if(block4)
        {
            BoardLocation location;

            location.x = itt->x;
            location.y = itt->y + 1;

            calculatePerimeterFeaturesForBlock(&state, block, block4, location);
        }
    }

    features->perimeter = state.perimeter.size();
    features->opponents = state.opponents.size();
    features->liberties = state.liberties.size();
    features->numberOfAdjacentOpponentBlocks = state.adjacentOpponentBlocks.size();
    features->boundingBoxSize = (maxX - minX + 1) * (maxY - minY + 1);

    itt = state.liberties.begin();
    end = state.liberties.end();

    for( ; itt != end; ++itt)
    {
        Block* block1 = getBlock(itt->x - 1, itt->y);
        Block* block2 = getBlock(itt->x, itt->y - 1);
        Block* block3 = getBlock(itt->x + 1, itt->y);
        Block* block4 = getBlock(itt->x, itt->y + 1);

        if(block1 && block1->getState() == EMPTY)
        {
            BoardLocation location;

            location.x = itt->x - 1;
            location.y = itt->y;

            if(state.liberties.find(location) == end)
            {
                state.secondOrderLiberties.insert(location);
            }
        }
        if(block2 && block2->getState() == EMPTY)
        {
            BoardLocation location;

            location.x = itt->x;
            location.y = itt->y - 1;

            if(state.liberties.find(location) == end)
            {
                state.secondOrderLiberties.insert(location);
            }
        }
        if(block3 && block3->getState() == EMPTY)
        {
            BoardLocation location;

            location.x = itt->x + 1;
            location.y = itt->y;

            if(state.liberties.find(location) == end)
            {
                state.secondOrderLiberties.insert(location);
            }
        }
        if(block4 && block4->getState() == EMPTY)
        {
            BoardLocation location;

            location.x = itt->x;
            location.y = itt->y + 1;

            if(state.liberties.find(location) == end)
            {
                state.secondOrderLiberties.insert(location);
            }
        }
    }

    features->secondOrderLiberties = state.secondOrderLiberties.size();

    itt = state.secondOrderLiberties.begin();
    end = state.secondOrderLiberties.end();

    for( ; itt != end; ++itt)
    {
        Block* block1 = getBlock(itt->x - 1, itt->y);
        Block* block2 = getBlock(itt->x, itt->y - 1);
        Block* block3 = getBlock(itt->x + 1, itt->y);
        Block* block4 = getBlock(itt->x, itt->y + 1);

        if(block1 && block1->getState() == EMPTY)
        {
            BoardLocation location;

            location.x = itt->x - 1;
            location.y = itt->y;

            if(state.liberties.find(location) == state.liberties.end() &&
               state.secondOrderLiberties.find(location) == end)
            {
                state.thirdOrderLiberties.insert(location);
            }
        }
        if(block2 && block2->getState() == EMPTY)
        {
            BoardLocation location;

            location.x = itt->x;
            location.y = itt->y - 1;

            if(state.liberties.find(location) == state.liberties.end() &&
               state.secondOrderLiberties.find(location) == end)
            {
                state.thirdOrderLiberties.insert(location);
            }
        }
        if(block3 && block3->getState() == EMPTY)
        {
            BoardLocation location;

            location.x = itt->x + 1;
            location.y = itt->y;

            if(state.liberties.find(location) == state.liberties.end() &&
               state.secondOrderLiberties.find(location) == end)
            {
                state.thirdOrderLiberties.insert(location);
            }
        }
        if(block4 && block4->getState() == EMPTY)
        {
            BoardLocation location;

            location.x = itt->x;
            location.y = itt->y + 1;

            if(state.liberties.find(location) == state.liberties.end() &&
               state.secondOrderLiberties.find(location) == end)
            {
                state.thirdOrderLiberties.insert(location);
            }
        }
    }

    features->thirdOrderLiberties = state.thirdOrderLiberties.size();

    itt = state.perimeter.begin();
    end = state.perimeter.end();

    std::set<BoardLocation> friendly;
    std::set<BoardLocation> enemy;

    for( ; itt != end; ++itt)
    {
        Block* block0 = getBlock(itt->x, itt->y);
        Block* block1 = getBlock(itt->x - 1, itt->y);
        Block* block2 = getBlock(itt->x, itt->y - 1);
        Block* block3 = getBlock(itt->x + 1, itt->y);
        Block* block4 = getBlock(itt->x, itt->y + 1);

        assert(block0);

        if(block0->getState() == block->getState())
        {
            friendly.insert(*itt);
        }
        else if(block0->getState() != EMPTY)
        {
            enemy.insert(*itt);
        }
        if(block1 && block1 != block)
        {
            BoardLocation location;

            location.x = itt->x - 1;
            location.y = itt->y;

            if(block1->getState() == block->getState())
            {
                friendly.insert(location);
            }
            else if(block1->getState() != EMPTY)
            {
                enemy.insert(location);
            }
        }
        if(block2 && block2 != block)
        {
            BoardLocation location;

            location.x = itt->x;
            location.y = itt->y - 1;

            if(block2->getState() == block->getState())
            {
                friendly.insert(location);
            }
            else if(block2->getState() != EMPTY)
            {
                enemy.insert(location);
            }
        }
        if(block3 && block3 != block)
        {
            BoardLocation location;

            location.x = itt->x + 1;
            location.y = itt->y;

            if(block3->getState() == block->getState())
            {
                friendly.insert(location);
            }
            else if(block3->getState() != EMPTY)
            {
                enemy.insert(location);
            }
        }
        if(block4 && block4 != block)
        {
            BoardLocation location;

            location.x = itt->x;
            location.y = itt->y + 1;

            if(block4->getState() == block->getState())
            {
                friendly.insert(location);
            }
            else if(block4->getState() != EMPTY)
            {
                enemy.insert(location);
            }
        }
    }

    features->localMajority = friendly.size() - enemy.size();
}

BlockFinalFeatures Board::generateFeatures(Block* block) const
{
    BlockFinalFeatures features;

    features.size = block->getSize();

    generatePerimeterFeatures(&features, block);

    return features;
}

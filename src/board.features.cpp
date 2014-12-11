#include "board.h"

#include <assert.h>
#include <math.h>
#include <set>

inline
void calculatePerimeterFeaturesForBlock(
        PerimeterFeatureState* state,
        Block* block0,
        Block* block,
        const BoardLocation& location)
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

void Board::calculateSecondOrderLiberties(
        PerimeterFeatureState* state,
        Block* block0,
        Block* block,
        const int x,
        const int y,
        bool* autoAtari) const
{
    if(block->getState() == EMPTY)
    {
        BoardLocation location(x, y);

        if(state->liberties.find(location) == state->liberties.end())
        {
            state->secondOrderLiberties.insert(location);
        }
    }
    else if(block->getState() != block0->getState() &&
            block0->getLiberties() == 2)
    {
        Block* block1 = getBlock(x - 1, y);
        Block* block2 = getBlock(x, y - 1);
        Block* block3 = getBlock(x + 1, y);
        Block* block4 = getBlock(x, y + 1);

        int count = 0;
        int perimeter = 0;

        if(block1)
        {
            if(block1->getState() == EMPTY && block0->touches(x, y) ||
               block1->getState() != block0->getState())
            {
                ++count;
            }

            ++perimeter;
        }
        if(block2)
        {
            if(block2->getState() == EMPTY && block0->touches(x, y) ||
               block2->getState() != block0->getState())
            {
                ++count;
            }

            ++perimeter;
        }
        if(block3)
        {
            if(block3->getState() == EMPTY && block0->touches(x, y) ||
               block3->getState() != block0->getState())
            {
                ++count;
            }

            ++perimeter;
        }
        if(block4)
        {
            if(block4->getState() == EMPTY && block0->touches(x, y) ||
               block4->getState() != block0->getState())
            {
                ++count;
            }

            ++perimeter;
        }

        if(count > perimeter - 2)
        {
            *autoAtari = true;
        }
    }
}

inline
void calculateProtectedBlock(
        Block* block0,
        Block* block,
        std::set<Block*>& lowLiberties,
        int& count)
{
    if(block->getState() != EMPTY)
    {
        if(block->getState() == block0->getState())
        {
            ++count;
        }
        else if(block->getState() != block0->getState() &&
                block->getLiberties() < 2)
        {
            lowLiberties.insert(block);
        }
    }
}

inline
int isShared(
        Block* block,
        std::set<Block*>& lowLiberties,
        const int& x,
        const int& y)
{
    if(block->getState() == EMPTY)
    {
        bool shared = false;

        std::set<Block*>::iterator itt = lowLiberties.begin();
        std::set<Block*>::iterator end = lowLiberties.end();

        for( ; itt != end; ++itt)
        {
            if((*itt)->touches(x, y))
            {
                shared = true;

                break;
            }
        }

        if(shared)
        {
            return 1;
        }
    }

    return 0;
}

bool Board::isProtected(Block* block, const int x, const int y) const
{
    Block* block1 = getBlock(x - 1, y);
    Block* block2 = getBlock(x, y - 1);
    Block* block3 = getBlock(x + 1, y);
    Block* block4 = getBlock(x, y + 1);

    int count = 0;
    int perimeter = 0;

    std::set<Block*> lowLiberties;

    if(block1)
    {
        calculateProtectedBlock(block, block1, lowLiberties, count);

        ++perimeter;
    }
    if(block2)
    {
        calculateProtectedBlock(block, block2, lowLiberties, count);

        ++perimeter;
    }
    if(block3)
    {
        calculateProtectedBlock(block, block3, lowLiberties, count);

        ++perimeter;
    }
    if(block4)
    {
        calculateProtectedBlock(block, block4, lowLiberties, count);

        ++perimeter;
    }

    if(lowLiberties.size() > 0)
    {
        int sharedCount = 0;
        int totalLiberties = -lowLiberties.size();

        if(block1)
        {
            sharedCount += isShared(block1, lowLiberties, x - 1, y);
            totalLiberties += block1->getLiberties();
        }
        if(block2)
        {
            sharedCount += isShared(block2, lowLiberties, x, y - 1);
            totalLiberties += block1->getLiberties();
        }
        if(block3)
        {
            sharedCount += isShared(block3, lowLiberties, x + 1, y);
            totalLiberties += block1->getLiberties();
        }
        if(block4)
        {
            sharedCount += isShared(block4, lowLiberties, x, y + 1);
            totalLiberties += block1->getLiberties();
        }

        if(totalLiberties - sharedCount < 2)
        {
            count += lowLiberties.size();
        }
    }
        
    if(count >= perimeter - 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

inline
void calculateThirdOrderLiberties(
        PerimeterFeatureState* state,
        Block* block,
        const int& x,
        const int& y)
{
    if(block->getState() == EMPTY)
    {
        BoardLocation location(x, y);

        if(state->liberties.find(location) == state->liberties.end() &&
           state->secondOrderLiberties.find(location) == state->secondOrderLiberties.end())
        {
            state->thirdOrderLiberties.insert(location);
        }
    }
}

inline
void calculateLocalMajority(
        PerimeterFeatureState* state,
        Block* block0,
        Block* block,
        const int& x,
        const int& y)
{
    if(block != block0)
    {
        BoardLocation location(x, y);

        if(block->getState() == block0->getState())
        {
            state->friendly.insert(location);
        }
        else if(block->getState() != EMPTY)
        {
            state->enemy.insert(location);
        }
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
    int sum = 0;

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
            BoardLocation location(itt->x - 1, itt->y);

            calculatePerimeterFeaturesForBlock(&state, block, block1, location);
        }
        if(block2)
        {
            BoardLocation location(itt->x, itt->y - 1);

            calculatePerimeterFeaturesForBlock(&state, block, block2, location);
        }
        if(block3)
        {
            BoardLocation location(itt->x + 1, itt->y);

            calculatePerimeterFeaturesForBlock(&state, block, block3, location);
        }
        if(block4)
        {
            BoardLocation location(itt->x, itt->y + 1);

            calculatePerimeterFeaturesForBlock(&state, block, block4, location);
        }

        int distances[2];

        int dx0 = itt->x;
        int dx1 = size - itt->x;
        int dy0 = itt->y;
        int dy1 = size - itt->y;

        if(dx0 < dx1)
        {
            distances[0] = dx0;
            distances[1] = dx1;
        }
        else
        {
            distances[0] = dx1;
            distances[1] = dx0;
        }

        if(dy0 < distances[0])
        {
            distances[1] = distances[0];
            distances[0] = dy0;
        }
        else if(dy0 < distances[1])
        {
            distances[1] = dy0;
        }

        if(dy1 < distances[1])
        {
            sum += dy1 + distances[0];
        }
        else
        {
            sum += distances[0] + distances[1];
        }
    }

    features->perimeter = state.perimeter.size();
    features->opponents = state.opponents.size();
    features->liberties = state.liberties.size();
    features->numberOfAdjacentOpponentBlocks = state.adjacentOpponentBlocks.size();
    features->centerOfMass = sum / (2.0 * block->getSize());
    features->boundingBoxSize = (maxX - minX + 1) * (maxY - minY + 1);

    features->protectedLiberties = 0;
    features->autoAtariLiberties = 0;

    itt = state.liberties.begin();
    end = state.liberties.end();

    for( ; itt != end; ++itt)
    {
        Block* block1 = getBlock(itt->x - 1, itt->y);
        Block* block2 = getBlock(itt->x, itt->y - 1);
        Block* block3 = getBlock(itt->x + 1, itt->y);
        Block* block4 = getBlock(itt->x, itt->y + 1);

        bool autoAtari = false;

        if(block1)
        {
            calculateSecondOrderLiberties(&state, block, block1,
                                          itt->x - 1, itt->y, &autoAtari);
        }
        if(block2)
        {
            calculateSecondOrderLiberties(&state, block, block2,
                                          itt->x, itt->y - 1, &autoAtari);
        }
        if(block3)
        {
            calculateSecondOrderLiberties(&state, block, block3,
                                          itt->x + 1, itt->y, &autoAtari);
        }
        if(block4)
        {
            calculateSecondOrderLiberties(&state, block, block4,
                                          itt->x, itt->y + 1, &autoAtari);
        }

        if(isProtected(block, itt->x, itt->y))
        {
            features->protectedLiberties += 1;
        }

        if(autoAtari)
        {
            features->autoAtariLiberties += 1;
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

        if(block1)
        {
            calculateThirdOrderLiberties(&state, block1, itt->x - 1, itt->y);
        }
        if(block2)
        {
            calculateThirdOrderLiberties(&state, block2, itt->x, itt->y - 1);
        }
        if(block3)
        {
            calculateThirdOrderLiberties(&state, block3, itt->x + 1, itt->y);
        }
        if(block4)
        {
            calculateThirdOrderLiberties(&state, block4, itt->x, itt->y + 1);
        }
    }

    features->thirdOrderLiberties = state.thirdOrderLiberties.size();

    itt = state.perimeter.begin();
    end = state.perimeter.end();

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
            state.friendly.insert(*itt);
        }
        else if(block0->getState() != EMPTY)
        {
            state.enemy.insert(*itt);
        }
        if(block1)
        {
            calculateLocalMajority(&state, block, block1, itt->x - 1, itt->y);
        }
        if(block2)
        {
            calculateLocalMajority(&state, block, block2, itt->x, itt->y - 1);
        }
        if(block3)
        {
            calculateLocalMajority(&state, block, block3, itt->x + 1, itt->y);
        }
        if(block4)
        {
            calculateLocalMajority(&state, block, block4, itt->x, itt->y + 1);
        }
    }

    features->localMajority = state.friendly.size() - state.enemy.size();
}

BlockFinalFeatures Board::generateFeatures(Block* block) const
{
    BlockFinalFeatures features;

    features.size = block->getSize();

    generatePerimeterFeatures(&features, block);

    return features;
}

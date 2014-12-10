#include "board.h"

#include <assert.h>
#include <math.h>
#include <set>

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

        if((!block1 || block1 == block) && (!block2 || block2 == block) &&
           (!block3 || block3 == block) && (!block4 || block4 == block))
        {
            features->protectedLiberties += 1;
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

    std::set<BoardLocation> friendly;
    std::set<BoardLocation> enemy;

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

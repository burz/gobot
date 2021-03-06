#include "board.h"

#include <cassert>
#include <cmath>
#include <set>

inline
void calculateLocalFeaturesForBlock(
        LocalFeatureState* state,
        Block* block0,
        Block* block,
        const BoardLocation& location)
{
    if(block != block0)
    {
        state->perimeter.insert(location);
    }
    if(block->getState() == EMPTY)
    {
        state->liberties.insert(location);

        state->adjacentTerritories.insert(block);
    }
    else if(block->getState() != block0->getState())
    {
        state->opponents.insert(location);

        state->adjacentOpponentBlocks.insert(block);
    }
}

inline
void handleTerritoryMember(
        std::set<Block*>& chain,
        Block* block,
        const int& x,
        const int& y,
        std::set<BoardLocation>& perimeter,
        bool& directLiberty,
        bool& friendlyLiberty,
        bool& enemyLiberty)
{
    if(block->getState() != EMPTY)
    {
        BoardLocation location(x, y);

        perimeter.insert(location);

        if(chain.find(block) != chain.end())
        {
            directLiberty = true;
        }
        else if(block->getState() == (*chain.begin())->getState())
        {
            friendlyLiberty = true;
        }
        else
        {
            enemyLiberty = true;
        }
    }
}

inline
int chooseSmallestDistances(
        const int size,
        const int x,
        const int y)
{
    int dx0 = x;
    int dx1 = size - x - 1;
    int dy0 = y;
    int dy1 = size - y - 1;

    int distances[2];

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
        return dy1 + distances[0];
    }
    else
    {
        return distances[0] + distances[1];
    }
}

void Board::getAdjacentSafeTerritories(
        std::set<Block*>& territories,
        Block* block,
        std::set<Block*>& skip) const
{
    std::set<Block*> blocks;

    getAdjacentBlocks(blocks, block);

    std::set<Block*>::iterator itt = blocks.begin();
    std::set<Block*>::iterator end = blocks.end();

    for( ; itt != end; ++itt)
    {
        if((skip.find(*itt) != skip.end() && block->numberOfTouches(*itt) < 2) ||
           (*itt)->getState() != EMPTY)
        {
            continue;
        }

        if((*itt)->getSize() == 1 &&
           isFalseEyeFor(*(*itt)->locationsBegin(), block->getState(), skip))
        {
            continue;
        }

        std::set<Block*> adjacentToTerritory;

        getAdjacentBlocks(adjacentToTerritory, *itt);

        std::set<Block*>::iterator adjItt = adjacentToTerritory.begin();
        std::set<Block*>::iterator adjEnd = adjacentToTerritory.end();

        bool safe = true;

        for( ; adjItt != adjEnd; ++adjItt)
        {
            if((*adjItt)->getState() != block->getState())
            {
                safe = false;

                break;
            }
        }

        if(safe)
        {
            territories.insert(*itt);
        }
    }
}

bool Board::isSafeFalseEyeFor(
        const BoardLocation& location,
        const SpaceState& state,
        std::set<Block*> skip) const
{
    Block* block0 = getBlock(location.x, location.y);

    skip.insert(block0);

    Block* block1 = getBlock(location.x - 1, location.y);
    Block* block2 = getBlock(location.x, location.y - 1);
    Block* block3 = getBlock(location.x + 1, location.y);
    Block* block4 = getBlock(location.x, location.y + 1);

    bool safe = true;

    if(block1 && block1->getState() != EMPTY && skip.find(block1) == skip.end())
    {
        skip.insert(block1);

        std::set<Block*> safeTerritories;

        getAdjacentSafeTerritories(safeTerritories, block1, skip);

        if(safeTerritories.size() == 0)
        {
            safe = false;
        }
    }
    if(block2 && block2->getState() != EMPTY && skip.find(block2) == skip.end())
    {
        skip.insert(block2);

        std::set<Block*> safeTerritories;

        getAdjacentSafeTerritories(safeTerritories, block2, skip);

        if(safeTerritories.size() == 0)
        {
            safe = false;
        }
    }
    if(block3 && block3->getState() != EMPTY && skip.find(block3) == skip.end())
    {
        skip.insert(block3);

        std::set<Block*> safeTerritories;

        getAdjacentSafeTerritories(safeTerritories, block3, skip);

        if(safeTerritories.size() == 0)
        {
            safe = false;
        }
    }
    if(block4 && block4->getState() != EMPTY && skip.find(block4) == skip.end())
    {
        skip.insert(block4);

        std::set<Block*> safeTerritories;

        getAdjacentSafeTerritories(safeTerritories, block4, skip);

        if(safeTerritories.size() == 0)
        {
            safe = false;
        }
    }

    return safe;
}

bool Board::isFalseEyeFor(
        const BoardLocation& location,
        const SpaceState& state,
        std::set<Block*>& skip) const
{
    Block* block1 = getBlock(location.x - 1, location.y - 1);
    Block* block2 = getBlock(location.x + 1, location.y - 1);
    Block* block3 = getBlock(location.x - 1, location.y + 1);
    Block* block4 = getBlock(location.x + 1, location.y + 1);

    int count = 0;
    int corners = 0;

    if(block1)
    {
        if(block1->getState() != state && block1->getState() != EMPTY)
        {
            ++count;
        }

        ++corners;
    }
    if(block2)
    {
        if(block2->getState() != state && block2->getState() != EMPTY)
        {
            ++count;
        }

        ++corners;
    }
    if(block3)
    {
        if(block3->getState() != state && block3->getState() != EMPTY)
        {
            ++count;
        }

        ++corners;
    }
    if(block4)
    {
        if(block4->getState() != state && block4->getState() != EMPTY)
        {
            ++count;
        }

        ++corners;
    }

    if((corners == 4 && count > 1) ||
       (corners != 4 && count > 0))
    {
        return !isSafeFalseEyeFor(location, state, skip);
    }
    else
    {
        return false;
    }
}

void Board::handleAdjacentTerritories(
        std::set<Block*>& chain,
        std::set<Block*>& adjacentTerritories,
        int& CETNumberOfTerritories,
        int& CETSize,
        int& CETPerimeter,
        float& CETCenterOfMass,
        int& ENumberOfBlocks,
        int& ESize,
        int& EPerimeter,
        int& DTNumberOfTerritories,
        int& DTDirectLiberties,
        int& DTLibertiesOfFriendlyBlocks,
        int& DTLibertiesOfEnemyBlocks) const
{
    CETNumberOfTerritories = 0;
    CETSize = 0;
    CETPerimeter = 0;
    ENumberOfBlocks = 0;
    ESize = 0;
    EPerimeter = 0;
    DTNumberOfTerritories = 0;
    DTDirectLiberties = 0;
    DTLibertiesOfFriendlyBlocks = 0;
    DTLibertiesOfEnemyBlocks = 0;

    if(adjacentTerritories.size() == 0)
    {
        return;
    }

    int sum = 0;

    std::set<Block*>::const_iterator itt = adjacentTerritories.begin();
    std::set<Block*>::const_iterator end = adjacentTerritories.end();

    SpaceState state = (*itt)->getState();

    for( ; itt != end; ++itt)
    {
        std::set<BoardLocation> perimeter;

        int directLiberties = 0;
        int friendlyLiberties = 0;
        int enemyLiberties = 0;

        std::set<BoardLocation>::const_iterator locationItt = (*itt)->locationsBegin();
        std::set<BoardLocation>::const_iterator locationEnd = (*itt)->locationsEnd();

        int partialSum = 0;

        for( ; locationItt != locationEnd; ++locationItt)
        {
            Block* block1 = getBlock(locationItt->x - 1, locationItt->y);
            Block* block2 = getBlock(locationItt->x, locationItt->y - 1);
            Block* block3 = getBlock(locationItt->x + 1, locationItt->y);
            Block* block4 = getBlock(locationItt->x, locationItt->y + 1);

            bool directLiberty = false;
            bool friendlyLiberty = false;
            bool enemyLiberty = false;

            if(block1)
            {
                handleTerritoryMember(chain, block1, locationItt->x - 1,
                                      locationItt->y, perimeter, directLiberty,
                                      friendlyLiberty, enemyLiberty);
            }
            if(block2)
            {
                handleTerritoryMember(chain, block2, locationItt->x,
                                      locationItt->y - 1, perimeter, directLiberty,
                                      friendlyLiberty, enemyLiberty);
            }
            if(block3)
            {
                handleTerritoryMember(chain, block3, locationItt->x + 1,
                                      locationItt->y, perimeter, directLiberty,
                                      friendlyLiberty, enemyLiberty);
            }
            if(block4)
            {
                handleTerritoryMember(chain, block4, locationItt->x,
                                      locationItt->y + 1, perimeter, directLiberty,
                                      friendlyLiberty, enemyLiberty);
            }

            if(directLiberty)
            {
                ++directLiberties;
            }
            if(friendlyLiberty)
            {
                ++friendlyLiberties;
            }
            if(enemyLiberty)
            {
                ++enemyLiberties;
            }

            partialSum += chooseSmallestDistances(size, locationItt->x, locationItt->y);
        }

        if(enemyLiberties == 0)
        {
            int size = (*itt)->getSize();

            ++CETNumberOfTerritories;
            CETSize += size;
            CETPerimeter += perimeter.size();

            sum += partialSum;

            if(size < 4)
            {
                if(size != 1 || !isFalseEyeFor(*(*itt)->locationsBegin(), state))
                {
                    ++ENumberOfBlocks;
                    ESize += size;
                    EPerimeter += perimeter.size();
                }
            }
        }
        else
        {
            ++DTNumberOfTerritories;
            DTDirectLiberties += directLiberties;
            DTLibertiesOfFriendlyBlocks += friendlyLiberties;
            DTLibertiesOfEnemyBlocks += enemyLiberties;
        }
    }

    if(CETSize)
    {
        CETCenterOfMass = sum / (2.0 * CETSize);
    }
    else
    {
        CETCenterOfMass = 0.0;
    }
}

inline
void updateCountAndPerimeter(
        Block* block0,
        Block*  block,
        const int& x,
        const int& y,
        int& count,
        int& perimeter)
{
    if((block->getState() == EMPTY && block0->touches(x, y)) ||
       block->getState() != block0->getState())
    {
        ++count;
    }

    ++perimeter;
}

void Board::calculateSecondOrderLiberties(
        LocalFeatureState* state,
        Block* block0,
        Block* block,
        const int x,
        const int y,
        bool& autoAtari,
        std::vector<Block*>& optimisticList) const
{
    if(block->getState() == EMPTY)
    {
        BoardLocation location(x, y);

        if(state->liberties.find(location) == state->liberties.end())
        {
            state->secondOrderLiberties.insert(location);
        }
    }
    else if(block->getState() == block0->getState() && block != block0 &&
            state->optimisticChain.find(block) == state->optimisticChain.end())
    {
        state->optimisticChain.insert(block);
        optimisticList.push_back(block);
    }
    else if(block0->getLiberties() == 2)
    {
        Block* block1 = getBlock(x - 1, y);
        Block* block2 = getBlock(x, y - 1);
        Block* block3 = getBlock(x + 1, y);
        Block* block4 = getBlock(x, y + 1);

        int count = 0;
        int perimeter = 0;

        if(block1)
        {
            updateCountAndPerimeter(block0, block1, x, y, count, perimeter);
        }
        if(block2)
        {
            updateCountAndPerimeter(block0, block2, x, y, count, perimeter);
        }
        if(block3)
        {
            updateCountAndPerimeter(block0, block3, x, y, count, perimeter);
        }
        if(block4)
        {
            updateCountAndPerimeter(block0, block4, x, y, count, perimeter);
        }

        if(count > perimeter - 2)
        {
            autoAtari = true;
        }
    }
}

int Board::lookupBlockInChainMap(Block* block) const
{
    int i = 0;

    std::vector<std::set<Block*> >::const_iterator itt = optimisticChains.begin();
    std::vector<std::set<Block*> >::const_iterator end = optimisticChains.end();

    for( ; itt != end; ++itt)
    {
        if(itt->find(block) != itt->end())
        {
            return i;
        }

        ++i;
    }

    return -1;
}

inline
void broadenOptimisticChain(
        LocalFeatureState* state,
        Block* block0,
        Block* block,
        std::vector<Block*>& optimisticList)
{
    if(block->getState() == EMPTY)
    {
        state->adjacentChainedTerritories.insert(block);
    }
    else if(block->getState() == block0->getState())
    {
        if(state->optimisticChain.find(block) == state->optimisticChain.end())
        {
            state->optimisticChain.insert(block);

            optimisticList.push_back(block);
        }
    }
    else if(!state->weakestAdjacentChainedBlock ||
            block->getLiberties() < state->weakestAdjacentChainedBlock->getLiberties())
    {
        state->weakestAdjacentChainedBlock = block;
    }
}

void Board::calculateOptimisticChain(
        LocalFeatureState* state,
        Block* block0,
        Block* block,
        const int x,
        const int y,
        std::set<BoardLocation>& perimeter,
        std::vector<Block*>& optimisticList) const
{
    BoardLocation location(x, y);

    perimeter.insert(location);

    if(block->getState() == EMPTY)
    {
        Block* block1 = getBlock(x - 1, y);
        Block* block2 = getBlock(x, y - 1);
        Block* block3 = getBlock(x + 1, y);
        Block* block4 = getBlock(x, y + 1);

        if(block1)
        {
            broadenOptimisticChain(state, block0, block1, optimisticList);
        }
        if(block2)
        {
            broadenOptimisticChain(state, block0, block2, optimisticList);
        }
        if(block3)
        {
            broadenOptimisticChain(state, block0, block3, optimisticList);
        }
        if(block4)
        {
            broadenOptimisticChain(state, block0, block4, optimisticList);
        }
    }
    else if(block->getState() != block0->getState() &&
           (!state->weakestAdjacentChainedBlock ||
            block->getLiberties() < state->weakestAdjacentChainedBlock->getLiberties()))
    {
        state->weakestAdjacentChainedBlock = block;
    }
}

void Board::generateOptimisticChain(
        BlockFinalFeatures* features,
        LocalFeatureState* state,
        Block* block,
        std::vector<Block*>& optimisticList) const
{
    features->OCSize = 0;

    std::set<BoardLocation> perimeter;

    while(optimisticList.size() > 0)
    {
        Block* chainedBlock = optimisticList.back();

        optimisticList.pop_back();

        features->OCSize += chainedBlock->getSize();

        std::set<BoardLocation>::const_iterator itt = chainedBlock->locationsBegin();
        std::set<BoardLocation>::const_iterator end = chainedBlock->locationsEnd();

        for( ; itt != end; ++itt)
        {
            Block* block1 = getBlock(itt->x - 1, itt->y);
            Block* block2 = getBlock(itt->x, itt->y - 1);
            Block* block3 = getBlock(itt->x + 1, itt->y);
            Block* block4 = getBlock(itt->x, itt->y + 1);

            if(block1 &&
               state->optimisticChain.find(block1) == state->optimisticChain.end())
            {
                calculateOptimisticChain(state, block, block1, itt->x - 1, itt->y,
                                         perimeter, optimisticList);
            }
            if(block2 &&
               state->optimisticChain.find(block2) == state->optimisticChain.end())
            {
                calculateOptimisticChain(state, block, block2, itt->x, itt->y - 1,
                                         perimeter, optimisticList);
            }
            if(block3 &&
               state->optimisticChain.find(block3) == state->optimisticChain.end())
            {
                calculateOptimisticChain(state, block, block3, itt->x + 1, itt->y,
                                         perimeter, optimisticList);
            }
            if(block4 &&
               state->optimisticChain.find(block4) == state->optimisticChain.end())
            {
                calculateOptimisticChain(state, block, block4, itt->x, itt->y + 1,
                                         perimeter, optimisticList);
            }
        }
    }

    features->OCNumberOfBlocks = state->optimisticChain.size();
    features->OCPerimeter = perimeter.size();
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
            totalLiberties += block2->getLiberties();
        }
        if(block3)
        {
            sharedCount += isShared(block3, lowLiberties, x + 1, y);
            totalLiberties += block3->getLiberties();
        }
        if(block4)
        {
            sharedCount += isShared(block4, lowLiberties, x, y + 1);
            totalLiberties += block4->getLiberties();
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
void calculateWeakEnemyPerimeter(
        int& liberties,
        int& sharedLiberties,
        Block* block0,
        Block* block,
        const int& x,
        const int& y,
        std::set<BoardLocation>& perimeter)
{
    BoardLocation location(x, y);

    if(perimeter.find(location) == perimeter.end())
    {
        perimeter.insert(location);

        if(block->getState() == EMPTY)
        {
            ++liberties;

            if(block0->touches(x, y))
            {
                ++sharedLiberties;
            }
        }
    }
}

void Board::calculateWeakEnemyFeatures(
        int& perimeter,
        int& liberties,
        int& sharedLiberties,
        Block* block0,
        Block* block) const
{
    liberties = 0;
    sharedLiberties = 0;

    std::set<BoardLocation> perimeterPoints;

    std::set<BoardLocation>::const_iterator itt = block->locationsBegin();
    std::set<BoardLocation>::const_iterator end = block->locationsEnd();

    for( ; itt != end; ++itt)
    {
        Block* block1 = getBlock(itt->x - 1, itt->y);
        Block* block2 = getBlock(itt->x, itt->y - 1);
        Block* block3 = getBlock(itt->x + 1, itt->y);
        Block* block4 = getBlock(itt->x, itt->y + 1);

        if(block1 && block1 != block)
        {
            calculateWeakEnemyPerimeter(liberties, sharedLiberties, block0, block1,
                                        itt->x - 1, itt->y, perimeterPoints);
        }
        if(block2 && block2 != block)
        {
            calculateWeakEnemyPerimeter(liberties, sharedLiberties, block0, block2,
                                        itt->x, itt->y - 1, perimeterPoints);
        }
        if(block3 && block3 != block)
        {
            calculateWeakEnemyPerimeter(liberties, sharedLiberties, block0, block3,
                                        itt->x + 1, itt->y, perimeterPoints);
        }
        if(block4 && block4 != block)
        {
            calculateWeakEnemyPerimeter(liberties, sharedLiberties, block0, block4,
                                        itt->x, itt->y + 1, perimeterPoints);
        }
    }

    perimeter = perimeterPoints.size();
}

void Board::generateWeakestEnemyFeatures(
        BlockFinalFeatures *features,
        LocalFeatureState* state,
        Block* block) const
{
    Block* weakestBlocks[] = { 0, 0 };

    std::set<Block*>::const_iterator itt = state->adjacentOpponentBlocks.begin();
    std::set<Block*>::const_iterator end = state->adjacentOpponentBlocks.end();

    for( ; itt != end; ++itt)
    {
        if(!weakestBlocks[0] ||
           (*itt)->getLiberties() < weakestBlocks[0]->getLiberties())
        {
            weakestBlocks[1] = weakestBlocks[0];
            weakestBlocks[0] = *itt;
        }
        else if(!weakestBlocks[1] ||
                (*itt)->getLiberties() < weakestBlocks[1]->getLiberties())
        {
            weakestBlocks[1] = *itt;
        }
    }

    if(weakestBlocks[0])
    {
        calculateWeakEnemyFeatures(features->WAEPerimeter, features->WAELiberties,
                                   features->WAESharedLiberties,
                                   block, weakestBlocks[0]);
    }
    else
    {
        features->WAEPerimeter = 0;
        features->WAELiberties = 0;
        features->WAESharedLiberties = 0;
    }
    if(weakestBlocks[1])
    {
        calculateWeakEnemyFeatures(features->SWAEPerimeter, features->SWAELiberties,
                                   features->SWAESharedLiberties,
                                   block, weakestBlocks[1]);
    }
    else
    {
        features->SWAEPerimeter = 0;
        features->SWAELiberties = 0;
        features->SWAESharedLiberties = 0;
    }
    if(state->weakestAdjacentChainedBlock)
    {
        calculateWeakEnemyFeatures(features->WACEPerimeter, features->WACELiberties,
                                   features->WACESharedLiberties,
                                   block, state->weakestAdjacentChainedBlock);
    }
    else
    {
        features->WACEPerimeter = 0;
        features->WACELiberties = 0;
        features->WACESharedLiberties = 0;
    }
}

inline
void calculateThirdOrderLiberties(
        LocalFeatureState* state,
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
        LocalFeatureState* state,
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

void Board::generateLocalFeatures(BlockFinalFeatures *features, Block* block)
{
    LocalFeatureState state;

    state.weakestAdjacentChainedBlock = 0;

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

            calculateLocalFeaturesForBlock(&state, block, block1, location);
        }
        if(block2)
        {
            BoardLocation location(itt->x, itt->y - 1);

            calculateLocalFeaturesForBlock(&state, block, block2, location);
        }
        if(block3)
        {
            BoardLocation location(itt->x + 1, itt->y);

            calculateLocalFeaturesForBlock(&state, block, block3, location);
        }
        if(block4)
        {
            BoardLocation location(itt->x, itt->y + 1);

            calculateLocalFeaturesForBlock(&state, block, block4, location);
        }

        sum += chooseSmallestDistances(size, itt->x, itt->y);
    }

    features->perimeter = state.perimeter.size();
    features->opponents = state.opponents.size();
    features->liberties = state.liberties.size();
    features->numberOfAdjacentOpponentBlocks = state.adjacentOpponentBlocks.size();
    features->centerOfMass = sum / (2.0 * block->getSize());
    features->boundingBoxSize = (maxX - minX + 1) * (maxY - minY + 1);

    std::set<Block*>* blockSet = new std::set<Block*>();

    blockSet->insert(block);

    handleAdjacentTerritories(*blockSet, state.adjacentTerritories,
                              features->CETNumberOfTerritories, features->CETSize,
                              features->CETPerimeter, features->CETCenterOfMass,
                              features->ENumberOfBlocks, features->ESize,
                              features->EPerimeter, features->DTNumberOfTerritories,
                              features->DTDirectLiberties,
                              features->DTLibertiesOfFriendlyBlocks,
                              features->DTLibertiesOfEnemyBlocks);

    delete blockSet;

    features->protectedLiberties = 0;
    features->autoAtariLiberties = 0;

    std::vector<Block*> optimisticList;

    state.optimisticChain.insert(block);

    optimisticList.push_back(block);

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
            calculateSecondOrderLiberties(&state, block, block1, itt->x - 1, itt->y,
                                          autoAtari, optimisticList);
        }
        if(block2)
        {
            calculateSecondOrderLiberties(&state, block, block2, itt->x, itt->y - 1,
                                          autoAtari, optimisticList);
        }
        if(block3)
        {
            calculateSecondOrderLiberties(&state, block, block3, itt->x + 1, itt->y,
                                          autoAtari, optimisticList);
        }
        if(block4)
        {
            calculateSecondOrderLiberties(&state, block, block4, itt->x, itt->y + 1,
                                          autoAtari, optimisticList);
        }

        if(isProtected(block, itt->x, itt->y))
        {
            ++features->protectedLiberties;
        }

        if(autoAtari)
        {
            ++features->autoAtariLiberties;
        }
    }

    features->secondOrderLiberties = state.secondOrderLiberties.size();

    int index = lookupBlockInChainMap(block);

    if(index != -1)
    {
        OptimisticChainFeatures OCFeatures = optimisticChainFeatures[index];

        features->OCNumberOfBlocks = OCFeatures.NumberOfBlocks;
        features->OCSize = OCFeatures.Size;
        features->OCPerimeter  = OCFeatures.Perimeter;
        features->OCCETNumberOfTerritories = OCFeatures.CETNumberOfTerritories;
        features->OCCETSize = OCFeatures.CETSize;
        features->OCCETPerimeter = OCFeatures.CETPerimeter;
        features->OCCETCenterOfMass = OCFeatures.CETCenterOfMass;
        features->OCENumberOfBlocks = OCFeatures.ENumberOfBlocks;
        features->OCESize = OCFeatures.ESize;
        features->OCEPerimeter = OCFeatures.EPerimeter;
        features->OCDTNumberOfTerritories = OCFeatures.DTNumberOfTerritories;
        features->OCDTDirectLiberties = OCFeatures.DTDirectLiberties;
        features->OCDTLibertiesOfFriendlyBlocks =
            OCFeatures.DTLibertiesOfFriendlyBlocks;
        features->OCDTLibertiesOfEnemyBlocks = OCFeatures.DTLibertiesOfEnemyBlocks;
    }
    else
    {
        generateOptimisticChain(features, &state, block, optimisticList);

        handleAdjacentTerritories(state.optimisticChain,
                                  state.adjacentChainedTerritories,
                                  features->OCCETNumberOfTerritories,
                                  features->OCCETSize, features->OCCETPerimeter,
                                  features->OCCETCenterOfMass,
                                  features->OCENumberOfBlocks, features->OCESize,
                                  features->OCEPerimeter,
                                  features->OCDTNumberOfTerritories,
                                  features->OCDTDirectLiberties,
                                  features->OCDTLibertiesOfFriendlyBlocks,
                                  features->OCDTLibertiesOfEnemyBlocks);

        OptimisticChainFeatures OCFeatures;

        OCFeatures.NumberOfBlocks = features->OCNumberOfBlocks;
        OCFeatures.Size = features->OCSize;
        OCFeatures.Perimeter = features->OCPerimeter;
        OCFeatures.CETNumberOfTerritories = features->OCCETNumberOfTerritories;
        OCFeatures.CETSize = features->OCCETSize;
        OCFeatures.CETPerimeter = features->OCCETPerimeter;
        OCFeatures.CETCenterOfMass = features->OCCETCenterOfMass;
        OCFeatures.ENumberOfBlocks = features->OCENumberOfBlocks;
        OCFeatures.ESize = features->OCESize;
        OCFeatures.EPerimeter = features->OCEPerimeter;
        OCFeatures.DTNumberOfTerritories = features->OCDTNumberOfTerritories;
        OCFeatures.DTDirectLiberties = features->OCDTDirectLiberties;
        OCFeatures.DTLibertiesOfFriendlyBlocks
            = features->OCDTLibertiesOfFriendlyBlocks;
        OCFeatures.DTLibertiesOfEnemyBlocks = features->OCDTLibertiesOfEnemyBlocks;

        optimisticChains.push_back(state.optimisticChain);
        optimisticChainFeatures.push_back(OCFeatures);
    }

    generateWeakestEnemyFeatures(features, &state, block);

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

BlockFinalFeatures Board::generateFinalFeatures(Block* block)
{
    if(modified)
    {
        optimisticChains.clear();
        optimisticChainFeatures.clear();

        modified = false;
    }

    BlockFinalFeatures features;

    features.size = block->getSize();

    generateLocalFeatures(&features, block);

    return features;
}

float* Board::generateFinalFeatureVector(Block* block)
{
    return getFeatureVector(generateFinalFeatures(block));
}

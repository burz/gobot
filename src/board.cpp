#include "board.h"

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <set>

Board::Board(const int _size, const float komi, const float _finalScore)
{
    size = _size;
    score = komi;
    finalScore = _finalScore;

    modified = true;

    spaces = new Space*[size]();

    Block* block = new Block();

    Space space(block);

    for(int i = 0; i < size; ++i)
    {
        spaces[i] = new Space[size];

        assert(spaces[i] != 0);

        for(int j = 0; j < size; ++j)
        {
            spaces[i][j] = space;

            BoardLocation location(i, j);

            block->add(location);
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
        delete[] spaces[i];
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

float Board::getFinalScore(void) const
{
    return finalScore;
}

SpaceState Board::getState(const int x, const int y) const
{
    return spaces[x][y].getState();
}

inline
int canCount(std::set<BoardLocation>& counted, const int& x, const int& y)
{
    BoardLocation location(x, y);

    if(counted.find(location) == counted.end())
    {
        counted.insert(location);

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
            result += canCount(counted, itt->x - 1, itt->y);
        }
        if(block2 && block2->getState() == EMPTY)
        {
            result += canCount(counted, itt->x, itt->y - 1);
        }
        if(block3 && block3->getState() == EMPTY)
        {
            result += canCount(counted, itt->x + 1, itt->y);
        }
        if(block4 && block4->getState() == EMPTY)
        {
            result += canCount(counted, itt->x, itt->y + 1);
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

    if(block->getState() == BLACK)
    {
        score += block->getSize();
    }
    else
    {
        score -= block->getSize();
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
    if(block1 && block1->getState() != EMPTY)
    {
        recalculateLiberties(block1);

        if(block1->getLiberties() == 0)
        {
            removeDeadGroup(block1);
        }
    }
    if(block2 && block2->getState() != EMPTY)
    {
        recalculateLiberties(block2);

        if(block2->getLiberties() == 0)
        {
            removeDeadGroup(block2);
        }
    }
    if(block3 && block3->getState() != EMPTY)
    {
        recalculateLiberties(block3);

        if(block3->getLiberties() == 0)
        {
            removeDeadGroup(block3);
        }
    }
    if(block4 && block4->getState() != EMPTY)
    {
        recalculateLiberties(block4);

        if(block4->getLiberties() == 0)
        {
            removeDeadGroup(block4);
        }
    }
}

inline
void fixBlockPointers(
        Block* block0,
        Block*& block1,
        Block*& block2,
        Block*& block3,
        Block*& block4)
{
    if(block1 == block0)
    {
        block1 = 0;
    }
    if(block2 == block0)
    {
        block2 = 0;
    }
    if(block3 == block0)
    {
        block3 = 0;
    }
    if(block4 == block0)
    {
        block4 = 0;
    }
}

void Board::playMove(const int x, const int y, const SpaceState state)
{
    BoardLocation location(x, y);

    Block* block0 = getBlock(x, y);
    Block* block1 = getBlock(x - 1, y);
    Block* block2 = getBlock(x, y - 1);
    Block* block3 = getBlock(x + 1, y);
    Block* block4 = getBlock(x, y + 1);

    assert(block0);

    block0->removeLocation(location);

    if(block0->getSize() == 0)
    {
        delete block0;
    }

    Block* currentBlock = new Block(state, location, 0);

    setBlock(x, y, currentBlock);

    if(block1 && handleAdjacentBlock(currentBlock, block1))
    {
        fixBlockPointers(block1, block1, block2, block3, block4);
    }
    if(block2 && handleAdjacentBlock(currentBlock, block2))
    {
        fixBlockPointers(block2, block1, block2, block3, block4);
    }
    if(block3 && handleAdjacentBlock(currentBlock, block3))
    {
        fixBlockPointers(block3, block1, block2, block3, block4);
    }
    if(block4 && handleAdjacentBlock(currentBlock, block4))
    {
        fixBlockPointers(block4, block1, block2, block3, block4);
    }

    recalculateLiberties(currentBlock);

    handlePossiblyDeadBlocks(block1, block2, block3, block4);

    modified = true;
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

void Board::setBlock(const BoardLocation& location, Block* block) const
{
    spaces[location.x][location.y].setBlock(block);
}

void Board::changeBlocks(Block* from, Block* to)
{
    for(int x = 0; x < size; ++x) {
        for(int y = 0; y < size; ++y) {
            spaces[x][y].changeBlock(from, to);
        }
    }
}

void Board::getBlocks(std::set<Block*>& blocks) const
{
    for(int x = 0; x < size; ++x) {
        for(int y = 0; y < size; ++y) {
            blocks.insert(getBlock(x, y));
        }
    }
}

void Board::getAdjacentBlocks(std::set<Block*>& blocks, Block* block) const
{
    std::set<BoardLocation>::iterator itt = block->locationsBegin();
    std::set<BoardLocation>::iterator end = block->locationsEnd();

    for( ; itt != end; ++itt)
    {
        Block* block1 = getBlock(itt->x - 1, itt->y);
        Block* block2 = getBlock(itt->x, itt->y - 1);
        Block* block3 = getBlock(itt->x + 1, itt->y);
        Block* block4 = getBlock(itt->x, itt->y + 1);

        if(block1 && block1 != block)
        {
            blocks.insert(block1);
        }
        if(block2 && block2 != block)
        {
            blocks.insert(block2);
        }
        if(block3 && block3 != block)
        {
            blocks.insert(block3);
        }
        if(block4 && block4 != block)
        {
            blocks.insert(block4);
        }
    }
}

void Board::splitEmptyBlocks(void)
{
    std::set<Block*> oldBlocks;
    std::set<Block*> newEmptyBlocks;

    getBlocks(oldBlocks);

    std::set<Block*>::iterator itt = oldBlocks.begin();
    std::set<Block*>::iterator end = oldBlocks.end();

    for( ; itt != end; ++itt)
    {
        if((*itt)->getState() == EMPTY)
        {
            std::set<BoardLocation>::iterator locationItt = (*itt)->locationsBegin();
            std::set<BoardLocation>::iterator locationEnd = (*itt)->locationsEnd();

            for( ; locationItt != locationEnd; ++locationItt)
            {
                Block* placed = 0;

                std::set<Block*> toDelete;

                std::set<Block*>::iterator emptyItt = newEmptyBlocks.begin();
                std::set<Block*>::iterator emptyEnd = newEmptyBlocks.end();

                for( ; emptyItt != emptyEnd; ++emptyItt)
                {
                    if((*emptyItt)->touches(*locationItt))
                    {
                        if(placed)
                        {
                            placed->absorb(*emptyItt);

                            changeBlocks(*emptyItt, placed);

                            toDelete.insert(*emptyItt);
                        }
                        else
                        {
                            (*emptyItt)->add(*locationItt);

                            setBlock(*locationItt, *emptyItt);

                            placed = *emptyItt;
                        }
                    }
                }

                std::set<Block*>::iterator toDeleteItt = toDelete.begin();
                std::set<Block*>::iterator toDeleteEnd = toDelete.end();

                for( ; toDeleteItt != toDeleteEnd; ++toDeleteItt)
                {
                    newEmptyBlocks.erase(*toDeleteItt);

                    delete *toDeleteItt;
                }

                if(!placed)
                {
                    Block* block = new Block(EMPTY);

                    block->add(*locationItt);

                    setBlock(*locationItt, block);

                    newEmptyBlocks.insert(block);
                }
            }

            delete *itt;
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

int Board::getCountableTerritory(
        Block* block,
        const SpaceState& state,
        std::map<BoardLocation, bool>& territoryMap) const
{
    int count = 0;

    std::set<BoardLocation>::iterator itt = block->locationsBegin();
    std::set<BoardLocation>::iterator end = block->locationsEnd();

    for( ; itt != end; ++itt)
    {
        if(!isFalseEyeFor(*itt, state))
        {
            if(state == BLACK)
            {
                std::pair<BoardLocation, bool> mapping(*itt, false);

                territoryMap.insert(mapping);
            }
            else
            {
                std::pair<BoardLocation, bool> mapping(*itt, true);

                territoryMap.insert(mapping);
            }

            ++count;
        }
    }

    return count;
}

float Board::calculateFinalScore(
        std::map<Block*, bool>& lifeMap,
        std::map<BoardLocation, bool>& territoryMap)
{
    std::set<Block*> blocks;
    float result = getScore();

    getBlocks(blocks);

    std::set<Block*>::iterator itt = blocks.begin();
    std::set<Block*>::iterator end = blocks.end();

    for( ; itt != end; ++itt)
    {
        if((*itt)->getState() != EMPTY)
        {
            if(!lifeMap.find(*itt)->second)
            {
                if((*itt)->getState() == BLACK)
                {
                    result += (*itt)->getSize();
                }
                else
                {
                    result -= (*itt)->getSize();
                }

                (*itt)->setState(EMPTY);
            }
        }
    }

    splitEmptyBlocks();

    getBlocks(blocks);

    itt = blocks.begin();
    end = blocks.end();

    for( ; itt != end; ++itt)
    {
        if((*itt)->getState() == EMPTY)
        {
            SpaceState adjacent = EMPTY;
            bool disputed = false;

            std::set<Block*> adjacentBlocks;

            getAdjacentBlocks(adjacentBlocks, *itt);

            std::set<Block*>::iterator blockItt = adjacentBlocks.begin();
            std::set<Block*>::iterator blockEnd = adjacentBlocks.end();

            for( ; blockItt != blockEnd; ++blockItt)
            {
                if((*blockItt)->getState() == BLACK)
                {
                    if(adjacent == WHITE)
                    {
                        disputed = true;

                        break;
                    }
                    else
                    {
                        adjacent = BLACK;
                    }
                }
            else
                {
                    if(adjacent == BLACK)
                    {
                        disputed = true;

                        break;
                    }
                    else
                    {
                        adjacent = WHITE;
                    }
                }
            }

            if(!disputed)
            {
                if(adjacent == BLACK)
                {
                    result -= getCountableTerritory(*itt, BLACK, territoryMap);
                }
                else
                {
                    result += getCountableTerritory(*itt, WHITE, territoryMap);
                }
            }
        }
    }

    return result;
}

bool Board::writeToFile(const char* filename)
{
    FILE *f = fopen(filename, "wb");

    if(!f)
    {
        return false;
    }

    std::set<Block*> blocks;

    getBlocks(blocks);

    int numberOfBlocks = blocks.size();

    if(fwrite(&size, sizeof(int), 1, f) != 1 ||
       fwrite(&score, sizeof(float), 1, f) != 1 ||
       fwrite(&finalScore, sizeof(float), 1, f) != 1 ||
       fwrite(&numberOfBlocks, sizeof(int), 1, f) != 1)
    {
        fclose(f);

        return false;
    }

    std::set<Block*>::iterator itt = blocks.begin();
    std::set<Block*>::iterator end = blocks.end();

    for( ; itt != end; ++itt)
    {
        int blockSize = (*itt)->getSize();
        SpaceState state = (*itt)->getState();
        int liberties = (*itt)->getLiberties();

        if(fwrite(&blockSize, sizeof(int), 1, f) != 1 ||
           fwrite(&state, sizeof(SpaceState), 1, f) != 1 ||
           fwrite(&liberties, sizeof(int), 1, f) != 1)
        {
            fclose(f);

            return false;
        }

        std::set<BoardLocation>::const_iterator locationItt = (*itt)->locationsBegin();
        std::set<BoardLocation>::const_iterator locationEnd = (*itt)->locationsEnd();

        for( ; locationItt != locationEnd; ++locationItt)
        {
            if(fwrite(&locationItt->x, sizeof(int), 1, f) != 1 ||
               fwrite(&locationItt->y, sizeof(int), 1, f) != 1)
            {
                fclose(f);

                return false;
            }
        }

        BlockFinalFeatures features = generateFinalFeatures(*itt);

        if(fwrite(&features, sizeof(BlockFinalFeatures), 1, f) != 1)
        {
            fclose(f);

            return false;
        }
    }

    fclose(f);

    return true;
}

bool Board::readFromFile(
        const char* filename,
        std::map<Block*, BlockFinalFeatures>& featureMap)
{
    FILE *f = fopen(filename, "rb");

    if(!f)
    {
        return false;
    }

    int numberOfBlocks;

    if(fread(&size, sizeof(int), 1, f) != 1 ||
       fread(&score, sizeof(float), 1, f) != 1 ||
       fread(&finalScore, sizeof(float), 1, f) != 1 ||
       fread(&numberOfBlocks, sizeof(int), 1, f) != 1)
    {
        fclose(f);

        return false;
    }

    spaces = new Space*[size]();

    for(int i = 0; i < size; ++i)
    {
        spaces[i] = new Space[size]();
    }

    for(int i = 0; i < numberOfBlocks; ++i)
    {
        int blockSize;
        SpaceState state;
        int liberties;

        if(fread(&blockSize, sizeof(int), 1, f) != 1 ||
           fread(&state, sizeof(SpaceState), 1, f) != 1 ||
           fread(&liberties, sizeof(int), 1, f) != 1)
        {
            fclose(f);

            return false;
        }

        Block* block = new Block(state);

        block->setLiberties(liberties);

        for(int j = 0; j < blockSize; ++j)
        {
            BoardLocation location(0, 0);

            if(fread(&location.x, sizeof(int), 1, f) != 1 ||
               fread(&location.y, sizeof(int), 1, f) != 1)
            {
                fclose(f);

                return false;
            }

            block->add(location);

            setBlock(location, block);
        }

        BlockFinalFeatures features;

        if(fread(&features, sizeof(BlockFinalFeatures), 1, f) != 1)
        {
            fclose(f);

            return false;
        }

        std::pair<Block*, BlockFinalFeatures> mapping(block, features);

        featureMap.insert(mapping);
    }

    fclose(f);

    return true;
}

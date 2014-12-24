#include "block.h"

#include <cmath>
#include <cstdio>

Block::Block(void)
{
    state = EMPTY;
    liberties = 0;
}

Block::Block(const SpaceState _state)
{
    state = _state;
    liberties = 0;
}

Block::Block(
        const SpaceState _state,
        const BoardLocation& location,
        const int _liberties)
{
    state = _state;

    liberties = _liberties;

    locations.insert(location);
}

bool Block::isMember(const SpaceState _state, const BoardLocation& location)
{
    if(state == _state)
    {
        return touches(location);
    }
    else if(state == EMPTY)
    {
        return contains(location);
    }
    else
    {
        return false;
    }
}

int Block::getSize(void) const
{
    return locations.size();
}

int Block::getLiberties(void) const
{
    return liberties;
}

void Block::setLiberties(const int _liberties)
{
    liberties = _liberties;
}

void Block::changeLiberties(const int value)
{
    liberties += value;
}

SpaceState Block::getState(void) const
{
    return state;
}

void Block::setState(const SpaceState _state)
{
    state = _state;
}

void Block::add(const BoardLocation& location)
{
    locations.insert(location);
}

bool Block::touches(const int x, const int y) const
{
    std::set<BoardLocation>::const_iterator itt = locations.begin();

    for( ; itt != locations.end(); ++itt)
    {
        BoardLocation blockMember = *itt;

        if(std::abs((float) blockMember.x - x) +
           std::abs((float) blockMember.y - y) == 1.0)
        {
            return true;
        }
    }

    return false;
}

bool Block::touches(const BoardLocation& location) const
{
    std::set<BoardLocation>::const_iterator itt = locations.begin();

    for( ; itt != locations.end(); ++itt)
    {
        if(std::abs((float) itt->x - location.x) +
           std::abs((float) itt->y - location.y) == 1.0)
        {
            return true;
        }
    }

    return false;
}

int Block::numberOfTouches(Block* block) const
{
    std::set<BoardLocation> touchLocations;

    std::set<BoardLocation>::const_iterator itt = locations.begin();
    std::set<BoardLocation>::const_iterator end = locations.end();

    for( ; itt != end; ++itt)
    {
        std::set<BoardLocation>::iterator blockItt = block->locationsBegin();
        std::set<BoardLocation>::iterator blockEnd = block->locationsEnd();

        for( ; blockItt != blockEnd; ++blockItt)
        {
            if(std::abs((float) blockItt->x - itt->x) +
               std::abs((float) blockItt->y - itt->y) == 1)
            {
                touchLocations.insert(*blockItt);
            }
        }
    }

    return touchLocations.size();
}

bool Block::contains(const BoardLocation& location) const
{
    std::set<BoardLocation>::const_iterator itt = locations.begin();

    for( ; itt != locations.end(); ++itt)
    {
        BoardLocation blockMember = *itt;

        if(location.x == blockMember.x && location.y == blockMember.y)
        {
            return true;
        }
    }

    return false;
}

void Block::removeLocation(const BoardLocation& location)
{
    std::set<BoardLocation>::iterator itt = locations.begin();

    for( ; itt != locations.end(); ++itt)
    {
        if(location.x == itt->x && location.y == itt->y)
        {
            locations.erase(*itt);

            break;
        }
    }
}

std::set<BoardLocation>::const_iterator Block::locationsBegin(void) const
{
    return locations.begin();
}

std::set<BoardLocation>::const_iterator Block::locationsEnd(void) const
{
    return locations.end();
}

void Block::absorb(Block* block)
{
    std::set<BoardLocation>::iterator itt = block->locations.begin();
    std::set<BoardLocation>::iterator end = block->locations.end();

    locations.insert(itt, end);
}

void Block::print(void) const
{
    std::set<BoardLocation>::const_iterator itt = locations.begin();

    for( ; itt != locations.end(); ++itt)
    {
        printf("(%d, %d), ", itt->x, itt->y);
    }

    printf("\b\b \n");
}

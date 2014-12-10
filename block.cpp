#include "block.h"

#include <math.h>

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

Block::Block(const SpaceState _state, const BoardLocation location, const int _liberties)
{
    state = _state;

    liberties = _liberties;

    locations.insert(location);
}

bool Block::isMember(const SpaceState _state, const BoardLocation location)
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

void Block::add(const BoardLocation location, int changeInLiberties)
{
    locations.insert(location);

    liberties += changeInLiberties;
}

bool Block::touches(const int x, const int y) const
{
    std::set<BoardLocation>::const_iterator itt = locations.begin();

    for( ; itt != locations.end(); ++itt)
    {
        BoardLocation blockMember = *itt;

        if(abs(blockMember.x - x) +
           abs(blockMember.y - y) == 1)
        {
            return true;
        }
    }

    return false;
}

bool Block::touches(const BoardLocation location) const
{
    std::set<BoardLocation>::const_iterator itt = locations.begin();

    for( ; itt != locations.end(); ++itt)
    {
        BoardLocation blockMember = *itt;

        if(abs(blockMember.x - location.x) +
           abs(blockMember.y - location.y) == 1)
        {
            return true;
        }
    }

    return false;
}

bool Block::contains(const BoardLocation location) const
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

void Block::removeLocation(const BoardLocation location)
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

    liberties += block->getLiberties();
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

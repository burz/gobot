#include "block.h"

#include <math.h>

Block::Block(void)
{
    liberties = 0;
    state = EMPTY;
}

Block::Block(const SpaceState state, const BoardLocation location, const int _liberties)
{
    liberties = _liberties;

    locations.push_back(location);
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

SpaceState Block::getState(void) const
{
    return state;
}

void Block::add(const BoardLocation location, int changeInLiberties)
{
    locations.push_back(location);

    liberties += changeInLiberties;
}

bool Block::touches(const BoardLocation location)
{
    std::vector<BoardLocation>::iterator itt = locations.begin();

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

bool Block::contains(const BoardLocation location)
{
    std::vector<BoardLocation>::iterator itt = locations.begin();

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
    std::vector<BoardLocation>::iterator itt = locations.begin();

    for( ; itt != locations.end(); ++itt)
    {
        if(location.x == itt->x && location.y == itt->y)
        {
            locations.erase(itt);

            break;
        }
    }
}

void Block::absorb(Block* block)
{
    std::vector<BoardLocation>::iterator itt = block->locations.begin();
    std::vector<BoardLocation>::iterator end = block->locations.end();

    locations.insert(locations.end(), itt, end);

    liberties += block->getLiberties() - 1;
}

void Block::print(void) const
{
    std::vector<BoardLocation>::const_iterator itt = locations.begin();

    for( ; itt != locations.end(); ++itt)
    {
        printf("(%d, %d), ", itt->x, itt->y);
    }

    printf("\b\b \n");
}

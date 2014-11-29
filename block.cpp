#include "block.h"

#include <math.h>

Block::Block(void)
{
    liberties = 0;
}

Block::Block(const SpaceState state, const BoardLocation location, const int _liberties)
{
    liberties = _liberties;

    locations.push_back(location);
}

int Block::getLiberties(void) const
{
    return liberties;
}

void Block::setLiberties(const int _liberties)
{
    liberties = _liberties;
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

void Block::absorb(Block* block)
{
    std::vector<BoardLocation>::iterator itt = block->locations.begin();
    std::vector<BoardLocation>::iterator end = block->locations.end();

    locations.insert(locations.end(), itt, end);

    --liberties;
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

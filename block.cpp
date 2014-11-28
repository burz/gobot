#include "block.h"

#include <math.h>

Block::Block(const BoardLocation location)
{
    locations.push_back(location);
}

bool Block::touches(const BoardLocation location)
{
    std::vector<BoardLocation>::iterator itt = locations.begin();

    for( ; itt != locations.end(); ++itt) {
        BoardLocation blockMember = *itt;

        if(abs(blockMember.x - location.x) +
           abs(blockMember.y - location.y) == 1)
        {
            return true;
        }
    }

    return false;
}

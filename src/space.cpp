#include "space.h"

Space::Space(Block* _block)
{
    block = _block;
}

SpaceState Space::getState(void) const
{
    return block->getState();
}

Block* Space::getBlock(void) const
{
    return block;
}

void Space::setBlock(Block* _block)
{
    block = _block;
}

void Space::changeBlock(Block* from, Block* to)
{
    if(block == from)
    {
        block = to;
    }
}

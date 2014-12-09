#include "space.h"

namespace {
const char emptyShorter[] = "E";
const char blackShorter[] = "B";
const char whiteShorter[] = "W";
const char empty[] = "EMPTY";
const char black[] = "BLACK";
const char white[] = "WHITE";
} // close anonymous namespace

const char* const spaceStateString(const SpaceState state, const bool shorter)
{
    if(shorter)
    {
        if(state == EMPTY) {
            return emptyShorter;
        }
        else if(state == BLACK) {
            return blackShorter;
        }
        else {
            return whiteShorter;
        }
    }
    else
    {
        if(state == EMPTY) {
            return empty;
        }
        else if(state == BLACK) {
            return black;
        }
        else {
            return white;
        }
    }   
}

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

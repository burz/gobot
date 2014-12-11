#include "space.h"

namespace {
const char emptyShorter[] = "E";
const char blackShorter[] = "B";
const char whiteShorter[] = "W";
const char empty[] = "EMPTY";
const char black[] = "BLACK";
const char white[] = "WHITE";
const char blackShorterColored[] = "\e[34mB\e[0m";
const char whiteShorterColored[] = "\e[93mW\e[0m";
} // close anonymous namespace

const char* spaceStateString(
        const SpaceState state,
        const bool shorter,
        const bool colored)
{
    if(colored)
    {
        if(state == EMPTY) {
            return emptyShorter;
        }
        else if(state == BLACK) {
            return blackShorterColored;
        }
        else {
            return whiteShorterColored;
        }
    }
    else if(shorter)
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

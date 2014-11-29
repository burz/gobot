#ifndef _SPACE_H_
#define _SPACE_H_

#include "definitions.h"
#include "block.h"

const char* const spaceStateString(const SpaceState state, const bool shorter = true);

class Space
{
  private:
    SpaceState state;
    Block* block;
  public:
    Space(Block* block);

    SpaceState getState(void) const;
    void changeState(const SpaceState state);

    Block* getBlock(void) const;
};

#endif

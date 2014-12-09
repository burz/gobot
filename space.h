#ifndef _SPACE_H_
#define _SPACE_H_

#include "definitions.h"
#include "block.h"

const char* const spaceStateString(const SpaceState state, const bool shorter = true);

class Space
{
  private:
    Block* block;
  public:
    Space(Block* block);

    SpaceState getState(void) const;

    Block* getBlock(void) const;
    void changeBlock(Block* from, Block* to);
};

#endif

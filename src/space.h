#ifndef _SPACE_H_
#define _SPACE_H_

#include "definitions.h"
#include "block.h"

class Space
{
  private:
    Block* block;
  public:
    Space(void) {}
    Space(Block* block);

    SpaceState getState(void) const;

    Block* getBlock(void) const;
    void setBlock(Block* block);
    void changeBlock(Block* from, Block* to);
};

#endif

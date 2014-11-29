#ifndef _BOARD_H_
#define _BOARD_H_

#include "space.h"
#include "block.h"

#include <vector>

class Board
{
  private:
    const int size;
    Space** spaces;
    std::vector<Block*> blocks;
  public:
    Board(const int size);
    ~Board(void);

    int getSize(void) const;
    SpaceState getState(const int x, const int y) const;
    void changeState(const int x, const int y, const SpaceState state);
    void print(void) const;
};

#endif

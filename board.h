#ifndef _BOARD_H_
#define _BOARD_H_

#include "space.h"

class Board
{
  private:
    const int size;
    Space** spaces;
  public:
    Board(const int size);
    ~Board(void);

    int getSize(void) const;
    SpaceState getState(const int x, const int y) const;
    void changeState(const int x, const int y, const SpaceState state);
    void print(void) const;
};

#endif

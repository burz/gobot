#ifndef _BOARD_H_
#define _BOARD_H_

#include "space.h"
#include "block.h"

class Board
{
  private:
    const int size;
    float score;
    Space** spaces;

    int updateAdjacentBlock(Block** currentBlock, Block* targetBlock);
  public:
    Board(const int size, const float komi);
    ~Board(void);

    int getSize(void) const;

    float getScore(void) const;

    SpaceState getState(const int x, const int y) const;

    void playMove(const int x, const int y, const SpaceState state);

    Block* getBlock(const int x, const int y) const;
    void setBlock(const int x, const int y, Block* block);
    void changeBlocks(Block* from, Block* to);

    void print(void) const;
};

#endif

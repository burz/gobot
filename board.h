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

    void removeDeadBlock(Block* block);
    void deleteAdjacentBlocks(Block* deleted,
                              Block** block1,
                              Block** block2,
                              Block** block3,
                              Block** block4);
    int checkIfEncountered(std::set<BoardLocation> *encounteredLocations,
                           const int x,
                           const int y);
    int adjacentLiberties(Block* block0, Block* block);
    void absorbAdjacentBlock(Block** currentBlock, Block* targetBlock, Block** deleted);
    Block* updateAdjacentBlock(Block** currentBlock, Block* targetBlock);
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

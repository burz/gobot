#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "definitions.h"

#include <vector>

typedef struct {
  int x;
  int y;
} BoardLocation;

class Block
{
  private:
    int liberties;
    SpaceState state;
    std::vector<BoardLocation> locations;
  public:
    Block(void);
    Block(const SpaceState state, const BoardLocation location, const int _liberties);

    int getLiberties(void) const;
    void setLiberties(const int liberties);

    void add(const BoardLocation location, int changeInLiberties);

    bool touches(const BoardLocation location);

    void absorb(Block* block);

    void print(void) const;
};

#endif

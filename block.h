#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "definitions.h"

#include <vector>

class Block
{
  private:
    int liberties;
    SpaceState state;
    std::vector<BoardLocation> locations;
  public:
    Block(void);
    Block(const SpaceState state, const BoardLocation location, const int _liberties);

    bool isMember(const SpaceState state, const BoardLocation location);

    int getSize(void) const;

    int getLiberties(void) const;
    void setLiberties(const int liberties);

    SpaceState getState(void) const;

    void add(const BoardLocation location, int changeInLiberties);

    bool touches(const BoardLocation location);
    bool contains(const BoardLocation location);

    void removeLocation(const BoardLocation location);

    void absorb(Block* block);

    void print(void) const;
};

#endif

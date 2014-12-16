#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "definitions.h"
#include "blockFeatures.h"

#include <set>

class Block
{
  private:
    int liberties;
    SpaceState state;
    std::set<BoardLocation> locations;
  public:
    Block(void);
    Block(const SpaceState state);
    Block(const SpaceState state, const BoardLocation& location, const int liberties);

    bool isMember(const SpaceState state, const BoardLocation& location);

    int getSize(void) const;

    int getLiberties(void) const;
    void setLiberties(const int liberties);
    void changeLiberties(const int value);

    SpaceState getState(void) const;
    void setState(const SpaceState state);

    void add(const BoardLocation& location);

    bool touches(const int x, const int y) const;
    bool touches(const BoardLocation& location) const;
    bool contains(const BoardLocation& location) const;

    void removeLocation(const BoardLocation& location);

    std::set<BoardLocation>::const_iterator locationsBegin(void) const;
    std::set<BoardLocation>::const_iterator locationsEnd(void) const;

    void absorb(Block* block);

    void print(void) const;
};

#endif

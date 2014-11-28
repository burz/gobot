#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <vector>

typedef struct {
  int x;
  int y;
} BoardLocation;

class Block
{
  private:
    std::vector<BoardLocation> locations;
  public:
    Block(const BoardLocation location);

    bool touches(const BoardLocation location);
};

#endif

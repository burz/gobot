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
    int liberties;
    std::vector<BoardLocation> locations;
  public:
    Block(const BoardLocation location, const int _liberties);

    int getLiberties(void) const;
    void setLiberties(const int liberties);

    bool touches(const BoardLocation location);

    void absorb(Block* block);

    void print(void) const;
};

#endif

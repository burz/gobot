#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

typedef enum {
    EMPTY = 0,
    BLACK,
    WHITE
} SpaceState;

class BoardLocation
{
  public:
    int x;
    int y;

    BoardLocation(const int x, const int y);
};

bool operator<(const BoardLocation& left, const BoardLocation& right);

#endif

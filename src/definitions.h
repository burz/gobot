#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

typedef enum {
    EMPTY = 0,
    BLACK,
    WHITE
} SpaceState;

const char* spaceStateString(
        const SpaceState state,
        const bool shorter = false,
        const bool colored = true);

class BoardLocation
{
  public:
    int x;
    int y;

    BoardLocation(const int x, const int y);
};

bool operator<(const BoardLocation& left, const BoardLocation& right);

#endif

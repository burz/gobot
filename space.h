#ifndef _SPACE_H_
#define _SPACE_H_

typedef enum {
    EMPTY = 0,
    BLACK,
    WHITE
} SpaceState;

const char* const spaceStateString(const SpaceState state, const bool shorter = true);

class Space
{
  private:
    SpaceState state;
  public:
    Space(void);

    SpaceState getState(void) const;
    void changeState(const SpaceState state);
};

#endif

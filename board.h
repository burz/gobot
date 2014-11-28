#ifndef _BOARD_H_
#define _BOARD_H_

typedef enum {
    EMPTY = 0,
    BLACK,
    WHITE
} SpaceState;

const char* const spaceStateString(const SpaceState state);

class Space
{
  private:
    SpaceState state;
  public:
    Space(void);

    SpaceState getState(void) const;
    void changeState(const SpaceState state);
};

class Board
{
  private:
    const int size;
    Space** spaces;
  public:
    Board(const int size);
    ~Board(void);

    int getSize(void) const;
    SpaceState getState(const int x, const int y) const;
    void changeState(const int x, const int y, const SpaceState state);
    void print(void) const;
};

#endif

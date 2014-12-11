#ifndef _GAME_H_
#define _GAME_H_

#include "definitions.h"

#include <vector>

class Game
{
  private:
    int size;
    float komi;
    float finalScore;
    std::vector<BoardLocation> moves;
  public:
    Game(void);
    Game(const int size, const float komi, const float finalScore);

    void addMove(const BoardLocation location);

    void playGame(bool printTurns = false) const;
};

#endif

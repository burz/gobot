#ifndef _GAME_H_
#define _GAME_H_

#include "definitions.h"
#include "block.h"
#include "board.h"

#include <vector>
#include <map>

class Game
{
  private:
    int size;
    float komi;
    float finalScore;
    std::vector<BoardLocation> moves;
    std::map<Block*, float*> featureMap;
  public:
    Game(void);
    Game(const int size, const float komi, const float finalScore);
    ~Game(void);

    void addMove(const BoardLocation location);

    Board playGame(void) const;

    void generateFeatureVectors(void);
};

#endif

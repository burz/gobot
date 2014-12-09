#ifndef _PARSER_H_
#define _PARSER_H_

#include "definitions.h"

#include <vector>

class Game
{
  public:
    float komi;
    float finalScore;
    std::vector<BoardLocation> moves;

    Game(void);
    Game(const float komi, const float finalScore);
};

bool parseFile(Game* game, const char *filename);

#endif

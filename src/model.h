#ifndef _MODEL_H_
#define _MODEL_H_

#include "game.h"

#include <vector>

class Model
{
  private:
  public:
    virtual void train(std::vector<Game>& games) = 0;
    virtual float run(const Game& game) const = 0;
    virtual float test(std::vector<Game>& games) const = 0;

    virtual bool outputToFile(const char* filename) const = 0;
    virtual bool readFromFile(const char* filename) = 0;
};

#endif

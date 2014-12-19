#ifndef _MODEL_H_
#define _MODEL_H_

#include "game.h"
#include "directoryLoader.h"

#include <vector>

class Model
{
  private:
  public:
    virtual void train(DirectoryIterator& boardFiles) = 0;
    virtual float predict(const char* boardFile) const = 0;
    virtual float test(DirectoryIterator& boardFiles) const = 0;

    virtual bool outputToFile(const char* filename) const = 0;
    virtual bool readFromFile(const char* filename) = 0;
};

#endif

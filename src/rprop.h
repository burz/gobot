#ifndef _RPROP_H_
#define _RPROP_H_

#include "model.h"
#include "rprop.layers.h"
#include "directoryLoader.h"

class RProp : public Model
{
  private:
    int inputSize;
    int hiddenSize;

    InputLayer inputLayer;
    InputBias inputBias;
    HiddenLayer hiddenLayer;
    HiddenBias hiddenBias;

    float calculateR(const float* features) const;

    float predict(const Board& board,
                  const std::map<Block*, BlockFinalFeatures>& featureMap) const;
  public:
    RProp(void) {}
    RProp(const int& inputSize, const int& hiddenSize);

    virtual void train(DirectoryIterator& boardFiles, const char* lifeDirectory);
    virtual float predict(const char* boardFile) const;
    virtual float test(DirectoryIterator& boardFiles) const;

    virtual bool writeToFile(const char* filename) const;
    virtual bool readFromFile(const char* filename);
};

#endif

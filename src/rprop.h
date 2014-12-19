#ifndef _RPROP_H_
#define _RPROP_H_

#include "model.h"
#include "rprop.weights.h"
#include "directoryLoader.h"

class RProp : public Model
{
  private:
    Weights* inputLayer;
    Weights* inputBias;
    Weights* hiddenLayer;
    Weights* hiddenBias;

    float calculateR(const float* features) const;

    float predict(const Board& board,
                  const std::map<Block*, BlockFinalFeatures>& featureMap) const;
  public:
    RProp(void) {}
    RProp(Weights* inputLayer,
          Weights* inputBias,
          Weights* hiddenLayer,
          Weights* hiddenBias);

    virtual void train(DirectoryIterator& boardFiles);
    virtual float predict(const char* boardFile) const;
    virtual float test(DirectoryIterator& boardFiles) const;

    virtual bool writeToFile(const char* filename) const;
    virtual bool readFromFile(const char* filename);
};

#endif

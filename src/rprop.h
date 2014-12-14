#ifndef _RPROP_H_
#define _RPROP_H_

#include "model.h"

class RProp : public Model
{
  private:
    int inputSize;
    int hiddenSize;
    float** bottomLayer;
    float* hiddenLayer;
    float** inputDerivative;
    float* hiddenDerivative;
    float** inputDelta;
    float* hiddenDelta;
    float inputDeltaMin;
    float inputDeltaMax;
    float hiddenDeltaMin;
    float hiddenDeltaMaX;
    float** inputDeltaW;
    float* hiddenDeltaW;
    float** lastInputDerivative;
    float* lastHiddenDerivative;
    float** lastInputDelta;
    float* lastHiddenDelta;
    float** lastInputDeltaW;
    float* lastHiddenDeltaW;

    void initializeTrainingParameters(void);
    void cleanUpTrainingParameters(void);
  public:
    RProp(void);
    RProp(const int& inputSize, const int& hiddenSize);
    ~RProp(void);

    virtual void train(std::vector<Game>& games, int iterations);
    virtual float test(std::vector<Game>& games) const;
    virtual float run(const Game& game) const;

    virtual bool outputToFile(const char* filename) const;
    virtual bool readFromFile(const char* filename);
};

#endif

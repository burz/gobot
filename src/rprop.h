#ifndef _RPROP_H_
#define _RPROP_H_

#include "model.h"

#define SECOND_INPUT_SIZE 5
#define SECOND_HIDDEN_SIZE 4

typedef enum {
    INPUT,
    HIDDEN,
    LASTINPUT,
    LASTHIDDEN
} ParameterType;

class RProp : public Model
{
  private:
    int inputSize;
    int hiddenSize;
    float** inputLayer;
    float* hiddenLayer;
    float* inputBias;
    float* hiddenBias;

    float** secondInputLayer;
    float* secondHiddenLayer;
    float* secondInputBias;
    float* secondHiddenBias;

    // Learning parameters
    float** inputDerivative;
    float** inputDelta;
    float** inputDeltaW;
    float** lastInputDerivative;
    float** lastInputDelta;
    float** lastInputDeltaW;

    float* hiddenDerivative;
    float* hiddenDelta;
    float* hiddenDeltaW;
    float* lastHiddenDerivative;
    float* lastHiddenDelta;
    float* lastHiddenDeltaW;

    float* inputBiasDerivative;
    float* inputBiasDelta;
    float* inputBiasDeltaW;
    float* lastInputBiasDerivative;
    float* lastInputBiasDelta;
    float* lastInputBiasDeltaW;

    float* hiddenBiasDerivative;
    float* hiddenBiasDelta;
    float* hiddenBiasDeltaW;
    float* lastHiddenBiasDerivative;
    float* lastHiddenBiasDelta;
    float* lastHiddenBiasDeltaW;

    float deltaMin;
    float deltaMax;

    void initializeTrainingParameters(void);
    void cleanUpTrainingParameters(void);

    float derivative(const ParameterType& type, const int& i, const int& j);
    float delta(const ParameterType& type, const int& i, const int& j);
    float deltaW(const ParameterType& type, const int& i, const int& j);
    float w(const ParameterType& type, const int& i, const int& j);

    void setDerivative(const ParameterType& type,
                       const int& i, const int& j, const float& d);
    void setDelta(const ParameterType& type,
                  const int& i, const int& j, const float& d);
    void setDeltaW(const ParameterType& type,
                    const int& i, const int& j, const float& d);

    float updateWeight(const ParameterType& type, const int& i, const int& j);

    float calculateR(float* features) const;

    float energyFunction(const Game& game) const;

    float runSecondPart(const Game& game,
                        const Board& board,
                        std::map<Block*, float> resultMap,
                        std::set<Block*> emptyBlocks) const;
  public:
    RProp(void);
    RProp(const int& inputSize, const int& hiddenSize);
    ~RProp(void);

    virtual void train(std::vector<Game>& games, const int& iterations);
    virtual float test(std::vector<Game>& games) const;
    virtual float run(const Game& game) const;

    virtual bool outputToFile(const char* filename) const;
    virtual bool readFromFile(const char* filename);
};

#endif

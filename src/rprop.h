#ifndef _RPROP_H_
#define _RPROP_H_

#include "model.h"

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
    float** inputDerivative;
    float* hiddenDerivative;
    float** inputDelta;
    float* hiddenDelta;
    float deltaMin;
    float deltaMax;
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
    float derivative(const ParameterType& type, const int& i, const int& j);
    float delta(const ParameterType& type, const int& i, const int& j);
    float deltaW(const ParameterType& type, const int& i, const int& j);
    void setDerivative(const ParameterType& type,
                       const int& i, const int& j, const float& d);
    void setDelta(const ParameterType& type,
                  const int& i, const int& j, const float& d);
    void setDeltaW(const ParameterType& type,
                    const int& i, const int& j, const float& d);
    float w(const ParameterType& type, const int& i, const int& j);
    float updateWeight(const ParameterType& type, const int& i, const int& j);
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

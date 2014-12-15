#include "rprop.h"

#include <math.h>

#define DELTA0 0.1
#define ETAPLUS 1.2
#define ETAMINUS 0.5

void RProp::initializeTrainingParameters(void)
{
    inputDerivative = new float*[hiddenSize]();
    inputDelta = new float*[hiddenSize]();
    inputDeltaW = new float*[hiddenSize]();
    lastInputDerivative = new float*[hiddenSize]();
    lastInputDelta = new float*[hiddenSize]();
    lastInputDeltaW = new float*[hiddenSize]();

    hiddenDerivative = new float[hiddenSize]();
    hiddenDelta = new float[hiddenSize]();
    hiddenDeltaW = new float[hiddenSize]();
    lastHiddenDerivative = new float[hiddenSize]();
    lastHiddenDelta = new float[hiddenSize]();
    lastHiddenDeltaW = new float[hiddenSize]();

    inputBiasDerivative = new float[inputSize]();
    inputBiasDelta = new float[inputSize]();
    inputBiasDeltaW = new float[inputSize]();
    lastInputBiasDerivative = new float[inputSize]();
    lastInputBiasDelta = new float[inputSize]();
    lastInputBiasDeltaW = new float[inputSize]();

    hiddenBiasDerivative = new float[hiddenSize]();
    hiddenBiasDelta = new float[hiddenSize]();
    hiddenBiasDeltaW = new float[hiddenSize]();
    lastHiddenBiasDerivative = new float[hiddenSize]();
    lastHiddenBiasDelta = new float[hiddenSize]();
    lastHiddenBiasDeltaW = new float[hiddenSize]();

    for(int i = 0; i < hiddenSize; ++i)
    {
        inputDerivative[i] = new float[inputSize]();
        inputDelta[i] = new float[inputSize]();
        inputDeltaW[i] = new float[inputSize]();
        lastInputDerivative[i] = new float[inputSize]();
        lastInputDelta[i] = new float[inputSize]();
        lastInputDeltaW[i] = new float[inputSize]();

        for(int j = 0; j < inputSize; ++j)
        {
            inputDerivative[i][j] = 1.0;
            inputDelta[i][j] = DELTA0;
            inputDeltaW[i][j] = DELTA0;
            lastInputDerivative[i][j] = 1.0;
            lastInputDelta[i][j] = DELTA0;
            lastInputDeltaW[i][j] = DELTA0;
        }

        hiddenDerivative[i] = 1.0;
        hiddenDelta[i] = DELTA0;
        hiddenDeltaW[i] = DELTA0;
        lastHiddenDerivative[i] = 1.0;
        lastHiddenDelta[i] = DELTA0;
        lastHiddenDeltaW[i] = DELTA0;

        hiddenBiasDerivative[i] = 1.0;
        hiddenBiasDelta[i] = DELTA0;
        hiddenBiasDeltaW[i] = DELTA0;
        lastHiddenBiasDerivative[i] = 1.0;
        lastHiddenBiasDelta[i] = DELTA0;
        lastHiddenBiasDeltaW[i] = DELTA0;
    }

    for(int i = 0; i < inputSize; ++i)
    {
        inputBiasDerivative[i] = 1.0;
        inputBiasDelta[i] = DELTA0;
        inputBiasDeltaW[i] = DELTA0;
        lastInputBiasDerivative[i] = 1.0;
        lastInputBiasDelta[i] = DELTA0;
        lastInputBiasDeltaW[i] = DELTA0;
    }

    secondInputDerivative = new float*[SECOND_HIDDEN_SIZE]();
    secondInputDelta = new float*[SECOND_HIDDEN_SIZE]();
    secondInputDeltaW = new float*[SECOND_HIDDEN_SIZE]();
    lastSecondInputDerivative = new float*[SECOND_HIDDEN_SIZE]();
    lastSecondInputDelta = new float*[SECOND_HIDDEN_SIZE]();
    lastSecondInputDeltaW = new float*[SECOND_HIDDEN_SIZE]();

    secondHiddenDerivative = new float[SECOND_HIDDEN_SIZE]();
    secondHiddenDelta = new float[SECOND_HIDDEN_SIZE]();
    secondHiddenDeltaW = new float[SECOND_HIDDEN_SIZE]();
    lastSecondHiddenDerivative = new float[SECOND_HIDDEN_SIZE]();
    lastSecondHiddenDelta = new float[SECOND_HIDDEN_SIZE]();
    lastSecondHiddenDeltaW = new float[SECOND_HIDDEN_SIZE]();

    secondInputBiasDerivative = new float[SECOND_INPUT_SIZE]();
    secondInputBiasDelta = new float[SECOND_INPUT_SIZE]();
    secondInputBiasDeltaW = new float[SECOND_INPUT_SIZE]();
    lastSecondInputBiasDerivative = new float[SECOND_INPUT_SIZE]();
    lastSecondInputBiasDelta = new float[SECOND_INPUT_SIZE]();
    lastSecondInputBiasDeltaW = new float[SECOND_INPUT_SIZE]();

    secondHiddenBiasDerivative = new float[SECOND_HIDDEN_SIZE]();
    secondHiddenBiasDelta = new float[SECOND_HIDDEN_SIZE]();
    secondHiddenBiasDeltaW = new float[SECOND_HIDDEN_SIZE]();
    lastSecondHiddenBiasDerivative = new float[SECOND_HIDDEN_SIZE]();
    lastSecondHiddenBiasDelta = new float[SECOND_HIDDEN_SIZE]();
    lastSecondHiddenBiasDeltaW = new float[SECOND_HIDDEN_SIZE]();

    for(int i = 0; i < SECOND_HIDDEN_SIZE; ++i)
    {
        secondInputDerivative[i] = new float[SECOND_INPUT_SIZE]();
        secondInputDelta[i] = new float[SECOND_INPUT_SIZE]();
        secondInputDeltaW[i] = new float[SECOND_INPUT_SIZE]();
        lastSecondInputDerivative[i] = new float[SECOND_INPUT_SIZE]();
        lastSecondInputDelta[i] = new float[SECOND_INPUT_SIZE]();
        lastSecondInputDeltaW[i] = new float[SECOND_INPUT_SIZE]();

        for(int j = 0; j < inputSize; ++j)
        {
            secondInputDerivative[i][j] = 1.0;
            secondInputDelta[i][j] = DELTA0;
            secondInputDeltaW[i][j] = DELTA0;
            lastSecondInputDerivative[i][j] = 1.0;
            lastSecondInputDelta[i][j] = DELTA0;
            lastSecondInputDeltaW[i][j] = DELTA0;
        }

        secondHiddenDerivative[i] = 1.0;
        secondHiddenDelta[i] = DELTA0;
        secondHiddenDeltaW[i] = DELTA0;
        lastSecondHiddenDerivative[i] = 1.0;
        lastSecondHiddenDelta[i] = DELTA0;
        lastSecondHiddenDeltaW[i] = DELTA0;

        secondHiddenBiasDerivative[i] = 1.0;
        secondHiddenBiasDelta[i] = DELTA0;
        secondHiddenBiasDeltaW[i] = DELTA0;
        lastSecondHiddenBiasDerivative[i] = 1.0;
        lastSecondHiddenBiasDelta[i] = DELTA0;
        lastSecondHiddenBiasDeltaW[i] = DELTA0;
    }

    for(int i = 0; i < inputSize; ++i)
    {
        secondInputBiasDerivative[i] = 1.0;
        secondInputBiasDelta[i] = DELTA0;
        secondInputBiasDeltaW[i] = DELTA0;
        lastSecondInputBiasDerivative[i] = 1.0;
        lastSecondInputBiasDelta[i] = DELTA0;
        lastSecondInputBiasDeltaW[i] = DELTA0;
    }

    deltaMin = DELTA0;
    deltaMax = DELTA0;
}

void RProp::cleanUpTrainingParameters(void)
{
    for(int i = 0; i < hiddenSize; ++i)
    {
        delete[] inputDerivative[i];
        delete[] inputDelta[i];
        delete[] inputDeltaW[i];
        delete[] lastInputDerivative[i];
        delete[] lastInputDelta[i];
        delete[] lastInputDeltaW[i];
    }

    delete[] inputDerivative;
    delete[] inputDelta;
    delete[] inputDeltaW;
    delete[] lastInputDerivative;
    delete[] lastInputDelta;
    delete[] lastInputDeltaW;

    delete[] hiddenDerivative;
    delete[] hiddenDelta;
    delete[] hiddenDeltaW;
    delete[] lastHiddenDerivative;
    delete[] lastHiddenDelta;
    delete[] lastHiddenDeltaW;

    delete[] inputBiasDerivative;
    delete[] inputBiasDelta;
    delete[] inputBiasDeltaW;
    delete[] lastInputBiasDerivative;
    delete[] lastInputBiasDelta;
    delete[] lastInputBiasDeltaW;

    delete[] hiddenBiasDerivative;
    delete[] hiddenBiasDelta;
    delete[] hiddenBiasDeltaW;
    delete[] lastHiddenBiasDerivative;
    delete[] lastHiddenBiasDelta;
    delete[] lastHiddenBiasDeltaW;

    for(int i = 0; i < hiddenSize; ++i)
    {
        delete[] secondInputDerivative[i];
        delete[] secondInputDelta[i];
        delete[] secondInputDeltaW[i];
        delete[] lastSecondInputDerivative[i];
        delete[] lastSecondInputDelta[i];
        delete[] lastSecondInputDeltaW[i];
    }

    delete[] secondInputDerivative;
    delete[] secondInputDelta;
    delete[] secondInputDeltaW;
    delete[] lastSecondInputDerivative;
    delete[] lastSecondInputDelta;
    delete[] lastSecondInputDeltaW;

    delete[] secondHiddenDerivative;
    delete[] secondHiddenDelta;
    delete[] secondHiddenDeltaW;
    delete[] lastSecondHiddenDerivative;
    delete[] lastSecondHiddenDelta;
    delete[] lastSecondHiddenDeltaW;

    delete[] secondInputBiasDerivative;
    delete[] secondInputBiasDelta;
    delete[] secondInputBiasDeltaW;
    delete[] lastSecondInputBiasDerivative;
    delete[] lastSecondInputBiasDelta;
    delete[] lastSecondInputBiasDeltaW;

    delete[] secondHiddenBiasDerivative;
    delete[] secondHiddenBiasDelta;
    delete[] secondHiddenBiasDeltaW;
    delete[] lastSecondHiddenBiasDerivative;
    delete[] lastSecondHiddenBiasDelta;
    delete[] lastSecondHiddenBiasDeltaW;
}

inline
float sign(const float& x)
{
    if(x == 0.0)
    {
        return 0.0;
    }
    else if(x > 0.0)
    {
        return 1.0;
    }
    else
    {
        return -1.0;
    }
}

float RProp::derivative(const ParameterType& type, const int& i, const int& j)
{
    if(type == INPUT)
    {
        return inputDerivative[i][j];
    }
    else if(type == HIDDEN)
    {
        return hiddenDerivative[i];
    }
    else if(type == INPUT_BIAS)
    {
        return inputBiasDerivative[i];
    }
    else if(type == HIDDEN_BIAS)
    {
        return hiddenBiasDerivative[i];
    }
    else if(type == LAST_INPUT)
    {
        return lastInputDerivative[i][j];
    }
    else if(type == LAST_HIDDEN)
    {
        return lastHiddenDerivative[i];
    }
    else if(type == LAST_INPUT_BIAS)
    {
        return lastInputBiasDerivative[i];
    }
    else
    {
        return lastHiddenBiasDerivative[i];
    }
}

float RProp::delta(const ParameterType& type, const int& i, const int& j)
{
    if(type == INPUT)
    {
        return inputDelta[i][j];
    }
    else if(type == HIDDEN)
    {
        return hiddenDelta[i];
    }
    else if(type == INPUT_BIAS)
    {
        return inputBiasDelta[i];
    }
    else if(type == HIDDEN_BIAS)
    {
        return hiddenBiasDelta[i];
    }
    else if(type == LAST_INPUT)
    {
        return lastInputDelta[i][j];
    }
    else if(type == LAST_HIDDEN)
    {
        return lastHiddenDelta[i];
    }
    else if(type == LAST_INPUT_BIAS)
    {
        return lastInputBiasDelta[i];
    }
    else
    {
        return lastHiddenBiasDelta[i];
    }
}

float RProp::deltaW(const ParameterType& type, const int& i, const int& j)
{
    if(type == INPUT)
    {
        return inputDeltaW[i][j];
    }
    else if(type == HIDDEN)
    {
        return hiddenDeltaW[i];
    }
    else if(type == INPUT_BIAS)
    {
        return inputBiasDeltaW[i];
    }
    else if(type == HIDDEN_BIAS)
    {
        return hiddenBiasDeltaW[i];
    }
    else if(type == LAST_INPUT)
    {
        return lastInputDeltaW[i][j];
    }
    else if(type == LAST_HIDDEN)
    {
        return lastHiddenDeltaW[i];
    }
    else if(type == LAST_INPUT_BIAS)
    {
        return lastInputBiasDeltaW[i];
    }
    else
    {
        return lastHiddenBiasDeltaW[i];
    }
}

float RProp::w(const ParameterType& type, const int& i, const int& j)
{
    if(type == INPUT)
    {
        return inputLayer[i][j];
    }
    else
    {
        return hiddenLayer[i];
    }
}

inline
ParameterType getLastType(const ParameterType& type)
{
    if(type == INPUT)
    {
        return LAST_INPUT;
    }
    else if(type == HIDDEN)
    {
        return LAST_HIDDEN;
    }
    else if(type == INPUT_BIAS)
    {
        return LAST_INPUT_BIAS;
    }
    else
    {
        return LAST_HIDDEN_BIAS;
    }
}

void RProp::setDerivative(
        const ParameterType& type,
        const int& i,
        const int& j,
        const float& d)
{
    if(type == INPUT)
    {
        inputDerivative[i][j] = d;
    }
    else if(type == HIDDEN)
    {
        hiddenDerivative[i] = d;
    }
    else if(type == INPUT_BIAS)
    {
        inputBiasDerivative[i] = d;
    }
    else
    {
        hiddenBiasDerivative[i] = d;
    }
}

void RProp::setDelta(
        const ParameterType& type,
        const int& i,
        const int& j,
        const float& d)
{
    if(type == INPUT)
    {
        inputDelta[i][j] = d;
    }
    else if(type == HIDDEN)
    {
        hiddenDelta[i] = d;
    }
    else if(type == INPUT_BIAS)
    {
        inputBiasDelta[i] = d;
    }
    else
    {
        hiddenBiasDelta[i] = d;
    }
}

void RProp::setDeltaW(
        const ParameterType& type,
        const int& i,
        const int& j,
        const float& d)
{
    if(type == INPUT)
    {
        inputDeltaW[i][j] = d;
    }
    else if(type == HIDDEN)
    {
        hiddenDeltaW[i] = d;
    }
    else if(type == INPUT_BIAS)
    {
        inputBiasDeltaW[i] = d;
    }
    else
    {
        hiddenBiasDeltaW[i] = d;
    }
}

float RProp::updateWeight(const ParameterType& type, const int& i, const int& j)
{
    ParameterType lastType = getLastType(type);

    float d = derivative(type, i, j) * derivative(lastType, i, j);

    if(d > 0.0)
    {
        float deltaT = fmin(ETAPLUS * delta(lastType, i, j), deltaMax);
        setDelta(type, i, j, deltaT);

        float deltaWT = -sign(derivative(type, i, j)) * deltaT;
        setDeltaW(type, i, j, deltaWT);

        return w(type, i, j) + deltaWT;
    }
    else if(d < 0.0)
    {
        float deltaT = fmax(ETAMINUS * delta(lastType, i, j), deltaMin);
        setDelta(type, i, j, deltaT);

        setDerivative(type, i, j, 0.0);

        return w(type, i, j) - w(lastType, i, j);
    }
    else
    {
        float deltaT = delta(lastType, i, j);
        setDelta(type, i, j, deltaT);

        float deltaWT = -sign(derivative(type, i, j)) * deltaT;
        setDeltaW(type, i, j, deltaWT);

        return w(type, i, j) + deltaWT;
    }
}

float RProp::calculateR(float* features) const
{
    float inputTemp[inputSize];
    float hiddenTemp[hiddenSize];

    for(int i = 0; i < inputSize; ++i)
    {
        inputTemp[i] = features[i] * inputBias[i];
    }

    for(int i = 0; i < hiddenSize; ++i)
    {
        float sum = 0.0;

        for(int j = 0; j < inputSize; ++j)
        {
            sum += inputTemp[j] * inputLayer[i][j];
        }

        hiddenTemp[i] = sum;
    }

    float result = 0.0;

    for(int i = 0; i < hiddenSize; ++i)
    {
        result += (hiddenTemp[i] + hiddenBias[i]) * hiddenLayer[i];
    }

    return result;
}

float RProp::energyFunction(const Game& game) const
{
    return abs(game.getFinalScore() - run(game));
}

void RProp::train(std::vector<Game>& games, const int& iterations)
{
    initializeTrainingParameters();

    for(int i = 0; i < iterations; ++i)
    {
        std::vector<Game>::iterator itt = games.begin();
        std::vector<Game>::iterator end = games.end();

        for( ; itt != end; ++itt)
        {
////
        }
    }

    cleanUpTrainingParameters();
}

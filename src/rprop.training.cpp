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

        for(int j = 0; j < SECOND_INPUT_SIZE; ++j)
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

    for(int i = 0; i < SECOND_INPUT_SIZE; ++i)
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

    for(int i = 0; i < SECOND_HIDDEN_SIZE; ++i)
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
    else if(type == LAST_HIDDEN_BIAS)
    {
        return lastHiddenBiasDerivative[i];
    }
    else if(type == SECOND_INPUT)
    {
        return secondInputDerivative[i][j];
    }
    else if(type == SECOND_HIDDEN)
    {
        return secondHiddenDerivative[i];
    }
    else if(type == SECOND_INPUT_BIAS)
    {
        return secondInputBiasDerivative[i];
    }
    else if(type == SECOND_HIDDEN_BIAS)
    {
        return secondHiddenBiasDerivative[i];
    }
    else if(type == LAST_SECOND_INPUT)
    {
        return lastSecondInputDerivative[i][j];
    }
    else if(type == LAST_SECOND_HIDDEN)
    {
        return lastSecondHiddenDerivative[i];
    }
    else if(type == LAST_SECOND_INPUT_BIAS)
    {
        return lastSecondInputBiasDerivative[i];
    }
    else
    {
        return lastSecondHiddenBiasDerivative[i];
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
    else if(type == LAST_HIDDEN_BIAS)
    {
        return lastHiddenBiasDelta[i];
    }
    else if(type == SECOND_INPUT)
    {
        return secondInputDelta[i][j];
    }
    else if(type == SECOND_HIDDEN)
    {
        return secondHiddenDelta[i];
    }
    else if(type == SECOND_INPUT_BIAS)
    {
        return secondInputBiasDelta[i];
    }
    else if(type == SECOND_HIDDEN_BIAS)
    {
        return secondHiddenBiasDelta[i];
    }
    else if(type == LAST_SECOND_INPUT)
    {
        return lastSecondInputDelta[i][j];
    }
    else if(type == LAST_SECOND_HIDDEN)
    {
        return lastSecondHiddenDelta[i];
    }
    else if(type == LAST_SECOND_INPUT_BIAS)
    {
        return lastSecondInputBiasDelta[i];
    }
    else
    {
        return lastSecondHiddenBiasDelta[i];
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
    else if(type == LAST_HIDDEN_BIAS)
    {
        return lastHiddenBiasDeltaW[i];
    }
    else if(type == SECOND_INPUT)
    {
        return secondInputDeltaW[i][j];
    }
    else if(type == SECOND_HIDDEN)
    {
        return secondHiddenDeltaW[i];
    }
    else if(type == SECOND_INPUT_BIAS)
    {
        return secondInputBiasDeltaW[i];
    }
    else if(type == SECOND_HIDDEN_BIAS)
    {
        return secondHiddenBiasDeltaW[i];
    }
    else if(type == LAST_SECOND_INPUT)
    {
        return lastSecondInputDeltaW[i][j];
    }
    else if(type == LAST_SECOND_HIDDEN)
    {
        return lastSecondHiddenDeltaW[i];
    }
    else if(type == LAST_SECOND_INPUT_BIAS)
    {
        return lastSecondInputBiasDeltaW[i];
    }
    else
    {
        return lastSecondHiddenBiasDeltaW[i];
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
    else if(type == HIDDEN_BIAS)
    {
        return LAST_HIDDEN_BIAS;
    }
    else if(type == SECOND_INPUT)
    {
        return LAST_SECOND_INPUT;
    }
    else if(type == SECOND_HIDDEN)
    {
        return LAST_SECOND_HIDDEN;
    }
    else if(type == SECOND_INPUT_BIAS)
    {
        return LAST_SECOND_INPUT_BIAS;
    }
    else
    {
        return LAST_SECOND_HIDDEN_BIAS;
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
    else if(type == HIDDEN_BIAS)
    {
        hiddenBiasDerivative[i] = d;
    }
    else if(type == SECOND_INPUT)
    {
        secondInputDerivative[i][j] = d;
    }
    else if(type == SECOND_HIDDEN)
    {
        secondHiddenDerivative[i] = d;
    }
    else if(type == SECOND_INPUT_BIAS)
    {
        secondInputBiasDerivative[i] = d;
    }
    else
    {
        secondHiddenBiasDerivative[i] = d;
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
    else if(type == HIDDEN_BIAS)
    {
        hiddenBiasDelta[i] = d;
    }
    else if(type == SECOND_INPUT)
    {
        secondInputDelta[i][j] = d;
    }
    else if(type == SECOND_HIDDEN)
    {
        secondHiddenDelta[i] = d;
    }
    else if(type == SECOND_INPUT_BIAS)
    {
        secondInputBiasDelta[i] = d;
    }
    else
    {
        secondHiddenBiasDelta[i] = d;
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
    else if(type == HIDDEN_BIAS)
    {
        hiddenBiasDeltaW[i] = d;
    }
    else if(type == SECOND_INPUT)
    {
        secondInputDeltaW[i][j] = d;
    }
    else if(type == SECOND_HIDDEN)
    {
        secondHiddenDeltaW[i] = d;
    }
    else if(type == SECOND_INPUT_BIAS)
    {
        secondInputBiasDeltaW[i] = d;
    }
    else
    {
        secondHiddenBiasDeltaW[i] = d;
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

float RProp::energyFunction(const Game& game) const
{
    return abs(game.getFinalScore() - predict(game));
}

void RProp::calculateDerivatives(
        const Board& board,
        std::vector<Block*>& emptyBlocks,
        std::map<Block*, float*>& featureMap)
{
    std::vector<float*> rVectors;

    for(int i = 0; i < hiddenSize; ++i)
    {
        for(int j = 0; j < inputSize; ++j)
        {
            inputDerivative[i][j] = 0.0;
        }

        hiddenDerivative[i] = 0.0;
        hiddenBiasDerivative[i] = 0.0;
    }

    for(int i = 0; i < inputSize; ++i)
    {
        inputBiasDerivative[i] = 0.0;
    }

    for(int i = 0; i < SECOND_HIDDEN_SIZE; ++i)
    {
        for(int j = 0; j < SECOND_INPUT_SIZE; ++j)
        {
            secondInputDerivative[i][j] = 0.0;
        }

        secondHiddenDerivative[i] = 0.0;
        secondHiddenBiasDerivative[i] = 0.0;
    }

    for(int i = 0; i < SECOND_INPUT_SIZE; ++i)
    {
        secondInputBiasDerivative[i] = 0.0;
    }

    std::vector<Block*>::iterator itt = emptyBlocks.begin();
    std::vector<Block*>::iterator end = emptyBlocks.end();

    for( ; itt != end; ++itt)
    {
        std::set<Block*> adjacentBlocks;

        board.getAdjacentBlocks(adjacentBlocks, *itt);

        float* r = new float[SECOND_INPUT_SIZE]();

        r[0] = r[1] = r[2] = r[3] = 0.0;

        std::set<Block*>::iterator blockItt = adjacentBlocks.begin();
        std::set<Block*>::iterator blockEnd = adjacentBlocks.end();

        for( ; blockItt != blockEnd; ++blockItt)
        {
            if((*blockItt)->getState() == BLACK)
            {
                r[0] += 1.0;
                r[2] += calculateR(featureMap.find(*blockItt)->second);
            }
            else
            {
                r[1] += 1.0;
                r[3] += calculateR(featureMap.find(*blockItt)->second);
            }
        }

        r[4] = (*itt)->getSize();

        rVectors.push_back(r);

        for(int i = 0; i < hiddenSize; ++i)
        {
            float dEdvNb = 0.0;
            float dEdvNw = 0.0;
            float dEdbNb = 0.0;
            float dEdbNw = 0.0;

            blockItt = adjacentBlocks.begin();

            for( ; blockItt != blockEnd; ++blockItt)
            {
                float* x = featureMap.find(*blockItt)->second;

                if((*blockItt)->getState() == BLACK)
                {
                    for(int j = 0; j < inputSize; ++j)
                    {
                        dEdvNb += inputLayer[i][j] * (x[j] + inputBias[j]);
                    }

                    dEdvNb += hiddenBias[i];
                    dEdbNb += hiddenLayer[i];
                }
                else
                {
                    for(int j = 0; j < inputSize; ++j)
                    {
                        dEdvNw += inputLayer[i][j] * (x[j] + inputBias[j]);
                    }

                    dEdvNw += hiddenBias[i];
                    dEdbNw += hiddenLayer[i];
                }
            }

            for(int j = 0; j < SECOND_HIDDEN_SIZE; ++j)
            {
                float sum = secondInputLayer[j][2] * dEdbNb +
                            secondInputLayer[j][3] * dEdbNw;

                hiddenBiasDerivative[i] -= secondHiddenLayer[j] * sum;

                sum = secondInputLayer[j][2] * dEdvNb +
                      secondInputLayer[j][3] * dEdvNw;

                hiddenDerivative[i] -= secondHiddenLayer[j] * sum;
            }

            for(int j = 0; j < inputSize; ++j)
            {
                float dEdwNb = 0.0;
                float dEdwNw = 0.0;

                blockItt = adjacentBlocks.begin();

                for( ; blockItt != blockEnd; ++blockItt)
                {
                    float* x = featureMap.find(*blockItt)->second;

                    if((*blockItt)->getState() == BLACK)
                    {
                        dEdwNb += hiddenLayer[i] * (x[j] + inputBias[j]);
                    }
                    else
                    {
                        dEdwNw += hiddenLayer[i] * (x[j] + inputBias[j]);
                    }
                }

                for(int k = 0; k < SECOND_HIDDEN_SIZE; ++k)
                {
                    float sum = secondInputLayer[k][2] * dEdwNb +
                                secondInputLayer[k][3] * dEdwNw;

                    inputDerivative[i][j] -= secondHiddenLayer[k] * sum;
                }
            }
        }

        for(int i = 0; i < inputSize; ++i)
        {
            float dEdaNb = 0.0;
            float dEdaNw = 0.0;

            blockItt = adjacentBlocks.begin();

            for( ; blockItt != blockEnd; ++blockItt)
            {
                if((*blockItt)->getState() == BLACK)
                {
                    for(int j = 0; j < hiddenSize; ++j)
                    {
                        dEdaNb += hiddenLayer[j] * inputLayer[j][i];
                    }
                }
                else
                {
                    for(int j = 0; j < hiddenSize; ++j)
                    {
                        dEdaNw += hiddenLayer[j] * inputLayer[j][i];
                    }
                }
            }

            for(int j = 0; j < SECOND_HIDDEN_SIZE; ++j)
            {
                float sum = secondInputLayer[j][2] * dEdaNb +
                            secondInputLayer[j][3] * dEdaNw;

                inputBiasDerivative[i] -= secondHiddenLayer[j] * sum;
            }
        }
    }

    int index = 0;

    itt = emptyBlocks.begin();

    for( ; itt != end; ++itt)
    {
        for(int i = 0; i < SECOND_HIDDEN_SIZE; ++i)
        {
            secondHiddenBiasDerivative[i] -= secondHiddenLayer[i];

            for(int j = 0; j < SECOND_INPUT_SIZE; ++j)
            {
                float rc = rVectors[index][j] + secondInputBias[j];

                secondHiddenDerivative[i] -= secondInputLayer[i][j] * rc;
                secondInputDerivative[i][j] -= secondHiddenLayer[i] * rc;
            }

            secondHiddenDerivative[i] -= hiddenBias[i];
        }

        for(int i = 0; i < SECOND_INPUT_SIZE; ++i)
        {
            for(int j = 0; j < SECOND_HIDDEN_SIZE; ++j)
            {
                secondInputBiasDerivative[i] = secondHiddenLayer[j] *
                                               secondInputLayer[j][i];
            }
        }

        ++index;
    }

    while(rVectors.size() > 0)
    {
        float* toDelete = rVectors.back();

        rVectors.pop_back();

        delete[] toDelete;
    }
}

inline
void swapPtrs(float**& x, float**& y)
{
    float** temp;
    temp = y;
    y = x;
    x = temp;
}

inline
void adjustBounds(const float& delta, float& deltaMin, float& deltaMax)
{
    if(delta < deltaMin)
    {
        deltaMin = delta;
    }
    else if(delta > deltaMax)
    {
        deltaMax = delta;
    }
}

inline
void swapPtrs(float*& x, float*& y)
{
    float* temp;
    temp = y;
    y = x;
    x = temp;
}

void RProp::runUpdates(
        const Board& board,
        std::vector<Block*>& emptyBlocks,
        std::map<Block*, float*>& featureMap)
{
    float** newInputLayer = new float*[hiddenSize]();
    float* newHiddenLayer = new float[hiddenSize]();
    float* newInputBias = new float[inputSize]();
    float* newHiddenBias = new float[hiddenSize]();
    float** newSecondInputLayer = new float*[SECOND_HIDDEN_SIZE]();
    float* newSecondHiddenLayer = new float[SECOND_HIDDEN_SIZE]();
    float* newSecondInputBias = new float[SECOND_INPUT_SIZE]();
    float* newSecondHiddenBias = new float[SECOND_HIDDEN_SIZE]();

    for(int i = 0; i < hiddenSize; ++i)
    {
        newInputLayer[i] = new float[inputSize];
    }

    for(int i = 0; i < SECOND_HIDDEN_SIZE; ++i)
    {
        newSecondInputLayer[i] = new float[SECOND_INPUT_SIZE];
    }

    calculateDerivatives(board, emptyBlocks, featureMap);

    for(int i = 0; i < hiddenSize; ++i)
    {
        for(int j = 0; j < inputSize; ++j)
        {
            newInputLayer[i][j] = updateWeight(INPUT, i, j);
        }

        newHiddenLayer[i] = updateWeight(HIDDEN, i);
        newHiddenBias[i] = updateWeight(HIDDEN_BIAS, i);
    }

    for(int i = 0; i < inputSize; ++i)
    {
        newInputBias[i] = updateWeight(INPUT_BIAS, i);
    }

    for(int i = 0; i < SECOND_HIDDEN_SIZE; ++i)
    {
        for(int j = 0; j < SECOND_INPUT_SIZE; ++j)
        {
            newSecondInputLayer[i][j] = updateWeight(SECOND_INPUT, i, j);
        }

        newSecondHiddenLayer[i] = updateWeight(SECOND_HIDDEN, i);
        newSecondHiddenBias[i] = updateWeight(SECOND_HIDDEN_BIAS, i);
    }

    for(int i = 0; i < SECOND_INPUT_SIZE; ++i)
    {
        newSecondInputBias[i] = updateWeight(SECOND_INPUT_BIAS, i);
    }

    deltaMin = deltaMax = hiddenDelta[0];

    for(int i = 0; i < hiddenSize; ++i)
    {
        for(int j = 0; j < inputSize; ++j)
        {
            adjustBounds(inputDelta[i][j], deltaMin, deltaMax);
        }

        adjustBounds(hiddenDelta[i], deltaMin, deltaMax);
        adjustBounds(hiddenBiasDelta[i], deltaMin, deltaMax);

        delete[] inputLayer[i];
    }

    for(int i = 0; i < inputSize; ++i)
    {
        adjustBounds(inputBiasDelta[i], deltaMin, deltaMax);
    }

    for(int i = 0; i < SECOND_HIDDEN_SIZE; ++i)
    {
        for(int j = 0; j < SECOND_INPUT_SIZE; ++j)
        {
            adjustBounds(secondInputDelta[i][j], deltaMin, deltaMax);
        }

        adjustBounds(secondHiddenDelta[i], deltaMin, deltaMax);
        adjustBounds(secondHiddenBiasDelta[i], deltaMin, deltaMax);
    }

    for(int i = 0; i < SECOND_INPUT_SIZE; ++i)
    {
        adjustBounds(secondInputBiasDelta[i], deltaMin, deltaMax);
    }

    delete[] inputLayer;
    delete[] hiddenLayer;
    delete[] inputBias;
    delete[] hiddenBias;

    inputLayer = newInputLayer;
    hiddenLayer = newHiddenLayer;
    inputBias = newInputBias;
    hiddenBias = newHiddenBias;

    swapPtrs(inputDerivative, lastInputDerivative);
    swapPtrs(inputDelta, lastInputDelta);
    swapPtrs(inputDelta, lastInputDeltaW);
    swapPtrs(hiddenDerivative, lastHiddenDerivative);
    swapPtrs(hiddenDelta, lastHiddenDelta);
    swapPtrs(hiddenDeltaW, lastHiddenDeltaW);
    swapPtrs(inputBiasDerivative, lastInputBiasDerivative);
    swapPtrs(inputBiasDelta, lastInputBiasDelta);
    swapPtrs(inputBiasDeltaW, lastInputBiasDeltaW);
    swapPtrs(hiddenBiasDerivative, lastHiddenBiasDerivative);
    swapPtrs(hiddenBiasDelta, lastHiddenBiasDelta);
    swapPtrs(hiddenBiasDeltaW, lastHiddenBiasDeltaW);
    swapPtrs(secondInputDerivative, lastSecondInputDerivative);
    swapPtrs(secondInputDelta, lastSecondInputDelta);
    swapPtrs(secondInputDelta, lastSecondInputDeltaW);
    swapPtrs(secondHiddenDerivative, lastSecondHiddenDerivative);
    swapPtrs(secondHiddenDelta, lastSecondHiddenDelta);
    swapPtrs(secondHiddenDeltaW, lastSecondHiddenDeltaW);
    swapPtrs(secondInputBiasDerivative, lastSecondInputBiasDerivative);
    swapPtrs(secondInputBiasDelta, lastSecondInputBiasDelta);
    swapPtrs(secondInputBiasDeltaW, lastSecondInputBiasDeltaW);
    swapPtrs(secondHiddenBiasDerivative, lastSecondHiddenBiasDerivative);
    swapPtrs(secondHiddenBiasDelta, lastSecondHiddenBiasDelta);
    swapPtrs(secondHiddenBiasDeltaW, lastSecondHiddenBiasDeltaW);
}

void RProp::train(std::vector<Game>& games, const int& iterations)
{
    initializeTrainingParameters();

    int t = 0;

    for(int i = 0; i < iterations; ++i)
    {
        std::vector<Game>::iterator itt = games.begin();
        std::vector<Game>::iterator end = games.end();

        for( ; itt != end; ++itt)
        {
            Board board = itt->playGame();

            std::set<Block*> blocks;

            board.getBlocks(blocks);

            std::vector<Block*> emptyBlocks;
            std::map<Block*, float*> featureMap;

            std::set<Block*>::iterator blockItt = blocks.begin();
            std::set<Block*>::iterator blockEnd = blocks.end();

            for( ; blockItt != blockEnd; ++blockItt)
            {
                if((*blockItt)->getState() == EMPTY)
                {
                    emptyBlocks.push_back(*blockItt);
                }
                else
                {
                    float* features = board.generateFinalFeatureVector(*blockItt);

                    std::pair<Block*, float*> mapping(*blockItt, features);

                    featureMap.insert(mapping);
                }
            }

            runUpdates(board, emptyBlocks, featureMap);

            if(t % 10 == 0)
            {
                printf(".");
                fflush(stdout);
            }

            ++t;
        }
    }

    printf("\n");

    cleanUpTrainingParameters();
}

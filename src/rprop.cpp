#include "rprop.h"

#include <math.h>

#define DELTA0 0.1
#define ETAPLUS 1.2
#define ETAMINUS 0.5

RProp::RProp(void)
{
    inputSize = 0;
}

inline
float randomFloatAroundZero(void)
{
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 2.0) - 1.0;
}

RProp::RProp(const int& _inputSize, const int& _hiddenSize)
{
    inputSize = _inputSize;
    hiddenSize = _hiddenSize;

    inputLayer = new float*[hiddenSize]();

    for(int i = 0; i < hiddenSize; ++i)
    {
        inputLayer[i] = new float[inputSize]();

        for(int j = 0; j < inputSize; ++j)
        {
            inputLayer[i][j] = randomFloatAroundZero();
        }
    }

    hiddenLayer = new float[hiddenSize]();
    inputBias = new float[inputSize]();
    hiddenBias = new float[hiddenSize]();

    for(int i = 0; i < hiddenSize; ++i)
    {
        hiddenLayer[i] = randomFloatAroundZero();
    }
}

RProp::~RProp(void)
{
    if(inputSize)
    {
        for(int i = 0; i < hiddenSize; ++i)
        {
            delete[] inputLayer[i];
        }

        delete[] inputLayer;
        delete[] hiddenLayer;
        delete[] inputBias;
        delete[] hiddenBias;
    }
}

void RProp::initializeTrainingParameters(void)
{
    inputDerivative = new float*[hiddenSize]();
    hiddenDerivative = new float[hiddenSize]();
    inputDelta = new float*[hiddenSize]();
    hiddenDelta = new float[hiddenSize]();
    inputDeltaW = new float*[hiddenSize]();
    hiddenDeltaW = new float[hiddenSize]();
    lastInputDerivative = new float*[hiddenSize]();
    lastHiddenDerivative = new float[hiddenSize]();
    lastInputDelta = new float*[hiddenSize]();
    lastHiddenDelta = new float[hiddenSize]();
    lastInputDeltaW = new float*[hiddenSize]();
    lastHiddenDeltaW = new float[hiddenSize]();

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
            lastInputDerivative[i][j] = 1.0;
            lastInputDelta[i][j] = DELTA0;
            lastInputDeltaW[i][j] = DELTA0;
        }

        lastHiddenDerivative[i] = 1.0;
        lastHiddenDelta[i] = DELTA0;
        lastHiddenDeltaW[i] = DELTA0;
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
    delete[] hiddenDerivative;
    delete[] inputDelta;
    delete[] hiddenDelta;
    delete[] inputDeltaW;
    delete[] hiddenDeltaW;
    delete[] lastInputDerivative;
    delete[] lastHiddenDerivative;
    delete[] lastInputDelta;
    delete[] lastHiddenDelta;
    delete[] lastInputDeltaW;
    delete[] lastHiddenDeltaW;
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
    else if(type == LASTINPUT)
    {
        return lastInputDerivative[i][j];
    }
    else
    {
        return lastHiddenDerivative[i];
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
    else if(type == LASTINPUT)
    {
        return lastInputDelta[i][j];
    }
    else
    {
        return lastHiddenDelta[i];
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
    else if(type == LASTINPUT)
    {
        return lastInputDeltaW[i][j];
    }
    else
    {
        return lastHiddenDeltaW[i];
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
        return LASTINPUT;
    }
    else
    {
        return LASTHIDDEN;
    }
}

void RProp::setDerivative(const ParameterType& type, const int& i, const int& j, const float& d)
{
    if(type == INPUT)
    {
        inputDerivative[i][j] = d;
    }
    else
    {
        hiddenDerivative[i] = d;
    }
}

void RProp::setDelta(const ParameterType& type, const int& i, const int& j, const float& d)
{
    if(type == INPUT)
    {
        inputDelta[i][j] = d;
    }
    else
    {
        hiddenDelta[i] = d;
    }
}

void RProp::setDeltaW(const ParameterType& type, const int& i, const int& j, const float& d)
{
    if(type == INPUT)
    {
        inputDeltaW[i][j] = d;
    }
    else
    {
        hiddenDeltaW[i] = d;
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

float RProp::test(std::vector<Game>& games) const
{
    if(games.size() > 0)
    {
        int correctPredictions = 0;
////
        return correctPredictions / ((float) games.size());
    }
    else
    {
        return 0.0;
    }
}

float RProp::runSecondPart(
        const Game& game,
        const Board& board,
        std::map<Block*, float> resultMap,
        std::set<Block*> emptyBlocks) const
{
    std::set<Block*>::iterator itt = emptyBlocks.begin();
    std::set<Block*>::iterator end = emptyBlocks.end();

    for( ; itt != end; ++itt)
    {
        std::set<Block*> adjacentBlocks;

        board.getAdjacentBlocks(adjacentBlocks, *itt);

        float secondInput[5] = { 0.0, 0.0, 0.0, 0.0, (*itt)->getSize() };

        std::set<Block*>::iterator adjacentItt = adjacentBlocks.begin();
        std::set<Block*>::iterator adjacentEnd = adjacentBlocks.end();

        for( ; adjacentItt != adjacentEnd; ++adjacentItt)
        {
////
        }
    }

    return 0.0;
}

float RProp::run(const Game& game) const
{
    Board board = game.playGame();

    std::map<Block*, float> resultMap;

    std::set<Block*> blocks;

    board.getBlocks(blocks);

    std::set<Block*> emptyBlocks;

    std::set<Block*>::const_iterator itt = blocks.begin();
    std::set<Block*>::const_iterator end = blocks.end();

    for( ; itt != end; ++itt)
    {
        if((*itt)->getState() != EMPTY)
        {
            float* features = board.generateFinalFeatureVector(*itt);

            std::pair<Block*, float> mapping(*itt, calculateR(features));

            resultMap.insert(mapping);

            delete[] features;
        }
        else
        {
            emptyBlocks.insert(*itt);
        }
    }

    return runSecondPart(game, board, resultMap, emptyBlocks);
}

bool RProp::outputToFile(const char* filename) const
{
    FILE* f = fopen(filename, "wb");

    if(!f)
    {
        return false;
    }

    if(fwrite(&inputSize, sizeof(int), 1, f) != 1 ||
       fwrite(&hiddenSize, sizeof(int), 1, f) != 1)
    {
        fclose(f);

        return false;
    }

    for(int i = 0; i < hiddenSize; ++i)
    {
        for(int j = 0; j < inputSize; ++j)
        {
            if(fwrite(&inputLayer[i][j], sizeof(float), 1, f) != 1)
            {
                fclose(f);

                return false;
            }
        }
    }

    for(int i = 0; i < hiddenSize; ++i)
    {
        if(fwrite(&hiddenLayer[i], sizeof(float), 1, f) != 1)
        {
            fclose(f);

            return false;
        }
    }

    fclose(f);

    return true;
}

bool RProp::readFromFile(const char* filename)
{
    FILE* f = fopen(filename, "rb");

    if(!f)
    {
        return false;
    }

    if(fread(&inputSize, sizeof(int), 1, f) != 1 ||
       fread(&hiddenSize, sizeof(int), 1, f) != 1)
    {
        fclose(f);

        return false;
    }

    for(int i = 0; i < hiddenSize; ++i)
    {
        for(int j = 0; j < inputSize; ++j)
        {
            if(fread(&inputLayer[i][j], sizeof(float), 1, f) != 1)
            {
                fclose(f);

                return false;
            }
        }
    }

    for(int i = 0; i < hiddenSize; ++i)
    {
        if(fread(&hiddenLayer[i], sizeof(float), 1, f) != 1)
        {
            fclose(f);

            return false;
        }
    }

    fclose(f);

    return true;
}

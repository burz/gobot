#include "rprop.h"

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

    bottomLayer = new float*[hiddenSize]();

    for(int i = 0; i < hiddenSize; ++i)
    {
        bottomLayer[i] = new float[inputSize]();

        for(int j = 0; j < inputSize; ++j)
        {
            bottomLayer[i][j] = randomFloatAroundZero();
        }
    }

    hiddenLayer = new float[hiddenSize]();

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
            delete[] bottomLayer[i];
        }

        delete[] bottomLayer;
        delete[] hiddenLayer;
    }
}

void RProp::initializeTrainingParameters(void)
{
    inputDerivative = new float*[hiddenSize]();
    inputDelta = new float*[hiddenSize]();
    inputDeltaW = new float*[hiddenSize]();
    lastInputDerivative = new float*[hiddenSize]();
    lastInputDelta = new float*[hiddenSize]();
    lastInputDeltaW = new float*[hiddenSize]();

    for(int i = 0; i < hiddenSize; ++i)
    {
        inputDerivative[i] = new float[inputSize]();
        inputDelta[i] = new float[inputSize]();
        inputDeltaW[i] = new float[inputSize]();
        lastInputDerivative[i] = new float[inputSize]();
        lastInputDelta[i] = new float[inputSize]();
        lastInputDeltaW[i] = new float[inputSize]();
    }
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
}

void RProp::train(std::vector<Game>& games, int iterations)
{
    initializeTrainingParameters();
////
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

float RProp::run(const Game& game) const
{
////
    return 0.0;
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
            if(fwrite(&bottomLayer[i][j], sizeof(float), 1, f) != 1)
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
            if(fread(&bottomLayer[i][j], sizeof(float), 1, f) != 1)
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

#include "rprop.h"

#include <math.h>

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

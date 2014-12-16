#include "rprop.h"
#include "parser.h"
#include "featureFile.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>

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
    hiddenLayer = new float[hiddenSize]();
    inputBias = new float[inputSize]();
    hiddenBias = new float[hiddenSize]();

    for(int i = 0; i < hiddenSize; ++i)
    {
        inputLayer[i] = new float[inputSize]();

        for(int j = 0; j < inputSize; ++j)
        {
            inputLayer[i][j] = randomFloatAroundZero();
        }

        hiddenLayer[i] = randomFloatAroundZero();
        hiddenBias[i] = randomFloatAroundZero();
    }

    for(int i = 0; i < inputSize; ++i)
    {
        inputBias[i] = randomFloatAroundZero();
    }

    secondInputLayer = new float*[SECOND_HIDDEN_SIZE];
    secondHiddenLayer = new float[SECOND_HIDDEN_SIZE];
    secondInputBias = new float[SECOND_INPUT_SIZE];
    secondHiddenBias = new float[SECOND_HIDDEN_SIZE];

    for(int i = 0; i < SECOND_HIDDEN_SIZE; ++i)
    {
        secondInputLayer[i] = new float[SECOND_INPUT_SIZE];

        for(int j = 0; j < SECOND_INPUT_SIZE; ++j)
        {
            secondInputLayer[i][j] = randomFloatAroundZero();
        }

        secondHiddenLayer[i] = randomFloatAroundZero();
        secondHiddenBias[i] = randomFloatAroundZero();
    }

    for(int i = 0; i < SECOND_INPUT_SIZE; ++i)
    {
        secondInputBias[i] = randomFloatAroundZero();
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

        for(int i = 0; i < SECOND_HIDDEN_SIZE; ++i)
        {
            delete[] secondInputLayer[i];
        }

        delete[] secondInputLayer;
        delete[] secondHiddenLayer;
        delete[] secondInputBias;
        delete[] secondHiddenBias;
    }
}

float RProp::calculateR(float* features) const
{
    float inputTemp[inputSize];
    float hiddenTemp[hiddenSize];

    for(int i = 0; i < inputSize; ++i)
    {
        inputTemp[i] = features[i] + inputBias[i];
    }

    for(int i = 0; i < hiddenSize; ++i)
    {
        float sum = 0.0;

        for(int j = 0; j < inputSize; ++j)
        {
            sum += inputLayer[i][j] * inputTemp[j];
        }

        hiddenTemp[i] = sum;
    }

    float result = 0.0;

    for(int i = 0; i < hiddenSize; ++i)
    {
        result += hiddenLayer[i] * (hiddenTemp[i] + hiddenBias[i]);
    }

    return result;
}

float RProp::calculateS(float* secondFeatures) const
{
    float inputTemp[SECOND_INPUT_SIZE];
    float hiddenTemp[SECOND_HIDDEN_SIZE];

    for(int i = 0; i < SECOND_INPUT_SIZE; ++i)
    {
        inputTemp[i] = secondFeatures[i] + secondInputBias[i];
    }

    for(int i = 0; i < SECOND_HIDDEN_SIZE; ++i)
    {
        float sum = 0.0;

        for(int j = 0; j < SECOND_INPUT_SIZE; ++j)
        {
            sum += secondInputLayer[i][j] * inputTemp[i];
        }

        hiddenTemp[i] = sum;
    }

    float result = 0.0;

    for(int i = 0; i < SECOND_HIDDEN_SIZE; ++i)
    {
        result += secondHiddenLayer[i] * (hiddenTemp[i] + secondHiddenBias[i]);
    }

    return result;
}

float RProp::runSecondPart(
        const Game& game,
        const Board& board,
        std::set<Block*>& blocks,
        std::map<Block*, float>& resultMap,
        std::set<Block*>& emptyBlocks) const
{
    float score = board.getScore();

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
            if((*adjacentItt)->getState() == BLACK)
            {
                ++secondInput[0];
                secondInput[2] += resultMap.find(*adjacentItt)->second;
            }
            else
            {
                ++secondInput[1];
                secondInput[3] += resultMap.find(*adjacentItt)->second;
            }
        }

        secondInput[4] = (*itt)->getSize();

        score += calculateS(secondInput);
    }

    itt = blocks.begin();
    end = blocks.end();

    for( ; itt != end; ++itt)
    {
        if((*itt)->getState() == BLACK && resultMap.find(*itt)->second < 0)
        {
            score -= 2.0 * (*itt)->getSize();
        }
        else if((*itt)->getState() == WHITE && resultMap.find(*itt)->second < 0)
        {
            score += 2.0 * (*itt)->getSize();
        }
    }

    return score;
}

float RProp::predictWithFeatures(
        const Game& game,
        std::map<BoardLocation, BlockFinalFeatures>& featureMap) const
{
    Board board = game.playGame();

    std::map<Block*, float> resultMap;

    std::map<BoardLocation, BlockFinalFeatures>::iterator itt = featureMap.begin();
    std::map<BoardLocation, BlockFinalFeatures>::iterator end = featureMap.end();

    for( ; itt != end; ++itt)
    {
        float* f = getFeatureVector(itt->second);

        std::pair<Block*, float> mapping(board.getBlock(itt->first), calculateR(f));

        resultMap.insert(mapping);

        delete[] f;
    }

    std::set<Block*> blocks;
    std::set<Block*> emptyBlocks;

    board.getBlocks(blocks);

    std::set<Block*>::const_iterator blockItt = blocks.begin();
    std::set<Block*>::const_iterator blockEnd = blocks.end();

    for( ; blockItt != blockEnd; ++blockItt)
    {
        if((*blockItt)->getState() == EMPTY)
        {
            emptyBlocks.insert(*blockItt);
        }
    }

    return runSecondPart(game, board, blocks, resultMap, emptyBlocks);
}

float RProp::predict(const Game& game) const
{
    Board board = game.playGame();

    std::map<Block*, float> resultMap;
    std::set<Block*> blocks;
    std::set<Block*> emptyBlocks;

    board.getBlocks(blocks);

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

    return runSecondPart(game, board, blocks, resultMap, emptyBlocks);
}

float RProp::test(DirectoryIterator& gameFiles) const
{
    int count = 0;
    int correctPredictions = 0;
    char buffer[100];

    DirectoryIterator end = DirectoryIterator::end();

    for( ; gameFiles != end; ++gameFiles)
    {
        Game game;

        sprintf(buffer, "%s/%s", gameFiles.getDirectory(), *gameFiles);

        if(!parseFile(&game, buffer))
        {
            continue;
        }

        if(predict(game) != game.getFinalScore())
        {
            ++correctPredictions;
        }

        ++count;
    }

    return static_cast<float>(correctPredictions) / static_cast<float>(count);
}

float RProp::test(std::vector<Game>& games) const
{
    if(games.size() > 0)
    {
        int correctPredictions = 0;

        std::vector<Game>::iterator itt = games.begin();
        std::vector<Game>::iterator end = games.end();

        for( ; itt != end; ++itt)
        {
            if(predict(*itt) != itt->getFinalScore())
            {
                ++correctPredictions;
            }
        }

        return static_cast<float>(correctPredictions) /
               static_cast<float>(games.size());
    }
    else
    {
        return 0.0;
    }
}

float RProp::testWithFeatures(
        DirectoryIterator& gameFiles,
        const char* featureFileDirectory) const
{
    int count = 0;
    int correctPredictions = 0;
    char buffer[100];

    DirectoryIterator end = DirectoryIterator::end();

    for( ; gameFiles != end; ++gameFiles)
    {
        Game game;

        sprintf(buffer, "%s/%s", gameFiles.getDirectory(), *gameFiles);

        if(!parseFile(&game, buffer))
        {
            continue;
        }

        sprintf(buffer, "%s/%sf", featureFileDirectory, *gameFiles);

        std::map<BoardLocation, BlockFinalFeatures> featureMap;

        if(!readFeaturesFromFile(featureMap, buffer))
        {
            continue;
        }

        if(predictWithFeatures(game, featureMap) != game.getFinalScore())
        {
            ++correctPredictions;
        }

        if(count % 10 == 0)
        {
            printf(".");
            fflush(stdout);
        }

        ++count;
    }

    printf("\n");

    return static_cast<float>(correctPredictions) / static_cast<float>(count);
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

        if(fwrite(&hiddenLayer[i], sizeof(float), 1, f) != 1 ||
           fwrite(&hiddenBias[i], sizeof(float), 1, f) != 1)
        {
            fclose(f);

            return false;
        }
    }

    for(int i = 0; i < inputSize; ++i)
    {
        if(fwrite(&inputBias[i], sizeof(float), 1, f) != 1)
        {
            fclose(f);

            return false;
        }
    }

    for(int i = 0; i < SECOND_HIDDEN_SIZE; ++i)
    {
        for(int j = 0; j < SECOND_INPUT_SIZE; ++j)
        {
            if(fwrite(&secondInputLayer[i][j], sizeof(float), 1, f) != 1)
            {
                fclose(f);

                return false;
            }
        }

        if(fwrite(&secondHiddenLayer[i], sizeof(float), 1, f) != 1 ||
           fwrite(&secondHiddenBias[i], sizeof(float), 1, f) != 1)
        {
            fclose(f);

            return false;
        }
    }

    for(int i = 0; i < SECOND_INPUT_SIZE; ++i)
    {
        if(fwrite(&secondInputBias[i], sizeof(float), 1, f) != 1)
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

    inputLayer = new float*[hiddenSize]();
    hiddenLayer = new float[hiddenSize]();
    inputBias = new float[inputSize]();
    hiddenBias = new float[hiddenSize]();

    for(int i = 0; i < hiddenSize; ++i)
    {
        inputLayer[i] = new float[inputSize]();
    }

    secondInputLayer = new float*[SECOND_HIDDEN_SIZE];
    secondHiddenLayer = new float[SECOND_HIDDEN_SIZE];
    secondInputBias = new float[SECOND_INPUT_SIZE];
    secondHiddenBias = new float[SECOND_HIDDEN_SIZE];

    for(int i = 0; i < SECOND_HIDDEN_SIZE; ++i)
    {
        secondInputLayer[i] = new float[SECOND_INPUT_SIZE];
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

        if(fread(&hiddenLayer[i], sizeof(float), 1, f) != 1 ||
           fread(&hiddenBias[i], sizeof(float), 1, f) != 1)
        {
            fclose(f);

            return false;
        }
    }

    for(int i = 0; i < inputSize; ++i)
    {
        if(fread(&inputBias[i], sizeof(float), 1, f) != 1)
        {
            fclose(f);

            return false;
        }
    }

    for(int i = 0; i < SECOND_HIDDEN_SIZE; ++i)
    {
        for(int j = 0; j < SECOND_INPUT_SIZE; ++j)
        {
            if(fread(&secondInputLayer[i][j], sizeof(float), 1, f) != 1)
            {
                fclose(f);

                return false;
            }
        }

        if(fread(&secondHiddenLayer[i], sizeof(float), 1, f) != 1 ||
           fread(&secondHiddenBias[i], sizeof(float), 1, f) != 1)
        {
            fclose(f);

            return false;
        }
    }

    for(int i = 0; i < SECOND_INPUT_SIZE; ++i)
    {
        if(fread(&secondInputBias[i], sizeof(float), 1, f) != 1)
        {
            fclose(f);

            return false;
        }
    }

    fclose(f);

    return true;
}

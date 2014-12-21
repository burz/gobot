#include "rprop.h"
#include "lifeFile.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>

RProp::RProp(const int& _inputSize, const int& _hiddenSize)
  : inputLayer(_hiddenSize, _inputSize), inputBias(_inputSize, 1),
    hiddenLayer(_hiddenSize, 1), hiddenBias(_hiddenSize, 1)
{
    inputSize = _inputSize;
    hiddenSize = _hiddenSize;
}

void RProp::train(DirectoryIterator& boardFiles, const char* lifeDirectory)
{
    char buffer[100];

    inputLayer.initializeForTraining();
    inputBias.initializeForTraining();
    hiddenLayer.initializeForTraining();
    hiddenBias.initializeForTraining();

    DirectoryIterator boardEnd = DirectoryIterator::end();

    for( ; boardFiles != boardEnd; ++boardFiles)
    {
        sprintf(buffer, "%s/%s", boardFiles.getDirectory(), *boardFiles);

        Board board;
        std::map<Block*, BlockFinalFeatures> featureMap;

        if(!board.readFromFile(buffer, featureMap))
        {
            continue;
        }

        sprintf(buffer, "%s/%sl", boardFiles.getDirectory(), *boardFiles);

        std::map<BoardLocation, bool> locationLifeMap;

        if(!readLifeFile(locationLifeMap, buffer))
        {
            continue;
        }

        std::map<Block*, bool> lifeMap;

        std::map<BoardLocation, bool>::iterator lifeItt = locationLifeMap.begin();
        std::map<BoardLocation, bool>::iterator lifeEnd = locationLifeMap.end();

        for( ; lifeItt != lifeEnd; ++lifeItt)
        {
            Block* block = board.getBlock(lifeItt->first);

            std::pair<Block*, bool> mapping(block, lifeItt->second);

            lifeMap.insert(mapping);
        }

        std::map<Block*, BlockFinalFeatures>::iterator itt = featureMap.begin();
        std::map<Block*, BlockFinalFeatures>::iterator end = featureMap.end();

        for( ; itt != end; ++itt)
        {
            float* features = getFeatureVector(itt->second);

            bool alive = lifeMap.find(itt->first)->second;

            inputLayer.update(*this, features, alive);
            inputBias.update(*this, features, alive);
            hiddenLayer.update(*this, features, alive);
            hiddenBias.update(*this, features, alive);

            delete[] features;
        }
    }

    inputLayer.cleanUpAfterTraining();
    inputBias.cleanUpAfterTraining();
    hiddenLayer.cleanUpAfterTraining();
    hiddenBias.cleanUpAfterTraining();
}

float RProp::calculateR(const float* features) const
{
    float inputTemp[inputSize];

    for(int i = 0; i < inputSize; ++i)
    {
        inputTemp[i] = features[i] + inputBias.getWeight(i);
    }

    float result = 0.0;

    for(int i = 0; i < hiddenSize; ++i)
    {
        float sum = 0.0;

        for(int j = 0; j < inputSize; ++j)
        {
            sum += inputLayer.getWeight(i, j) * inputTemp[j];
        }

        result += hiddenLayer.getWeight(i) * (sum + hiddenBias.getWeight(i));
    }

    return result;
}

float RProp::predict(
        Board& board,
        const std::map<Block*, BlockFinalFeatures>& featureMap) const
{
    std::set<Block*> blocks;

    board.getBlocks(blocks);

    std::map<Block*, bool> lifeMap;

    std::set<Block*>::iterator itt = blocks.begin();
    std::set<Block*>::iterator end = blocks.end();

    for( ; itt != end; ++itt)
    {
        if((*itt)->getState() != EMPTY)
        {
            float* features = getFeatureVector(featureMap.find(*itt)->second);

            float r = calculateR(features);

            delete[] features;

            bool alive = r >= 0;

            std::pair<Block*, bool> mapping(*itt, alive);

            lifeMap.insert(mapping);
        }
    }

    return board.calculateFinalScore(lifeMap);
}

float RProp::predict(const char* boardFile) const
{
    Board board;
    std::map<Block*, BlockFinalFeatures> featureMap;

    board.readFromFile(boardFile, featureMap);

    return predict(board, featureMap);
}

float RProp::test(DirectoryIterator& boardFiles) const
{
    int count = 0;
    int correctPredictions = 0;
    char buffer[100];

    DirectoryIterator end = DirectoryIterator::end();

    for( ; boardFiles != end; ++boardFiles)
    {
        sprintf(buffer, "%s/%s", boardFiles.getDirectory(), *boardFiles);

        Board board;
        std::map<Block*, BlockFinalFeatures> featureMap;

        board.readFromFile(buffer, featureMap);

        if(predict(buffer) == board.getFinalScore())
        {
            ++correctPredictions;
        }

        ++count;
    }

    return static_cast<float>(correctPredictions) / static_cast<float>(count);
}

bool RProp::writeToFile(const char* filename) const
{
    FILE* f = fopen(filename, "wb");

    if(!f)
    {
        return false;
    }

    if(!inputLayer.writeToFile(f) ||
       !inputBias.writeToFile(f) ||
       !hiddenLayer.writeToFile(f) ||
       !hiddenBias.writeToFile(f))
    {
        fclose(f);

        return false;
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

    if(!inputLayer.readFromFile(f) ||
       !inputBias.readFromFile(f) ||
       !hiddenLayer.readFromFile(f) ||
       !hiddenBias.readFromFile(f))
    {
        fclose(f);

        return false;
    }

    inputSize = inputLayer.getWidth();
    hiddenSize = inputLayer.getHeight();

    fclose(f);

    return true;
}

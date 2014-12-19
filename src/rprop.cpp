#include "rprop.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>

RProp::RProp(Weights* _inputLayer,
             Weights* _inputBias,
             Weights* _hiddenLayer,
             Weights* _hiddenBias)
{
    inputLayer = _inputLayer;
    inputBias = _inputBias;
    hiddenLayer = _hiddenLayer;
    hiddenBias = _hiddenBias;
}

void RProp::train(DirectoryIterator& boardFiles)
{
    char buffer[100];

    inputLayer->initializeForTraining();
    inputBias->initializeForTraining();
    hiddenLayer->initializeForTraining();
    hiddenBias->initializeForTraining();

    DirectoryIterator boardEnd = DirectoryIterator::end();

    for( ; boardFiles != boardEnd; ++boardFiles)
    {
        sprintf(buffer, "%s/%s", boardFiles.getDirectory(), *boardFiles);

        Board board;
        std::map<Block*, BlockFinalFeatures> featureMap;

        board.readFromFile(buffer, featureMap);

        std::map<Block*, BlockFinalFeatures>::iterator itt = featureMap.begin();
        std::map<Block*, BlockFinalFeatures>::iterator end = featureMap.end();

        for( ; itt != end; ++itt)
        {
            float* features = getFeatureVector(itt->second);

            inputLayer->update(features);
            inputBias->update(features);
            hiddenLayer->update(features);
            hiddenBias->update(features);

            delete[] features;
        }
    }

    inputLayer->cleanUpAfterTraining();
    inputBias->cleanUpAfterTraining();
    hiddenLayer->cleanUpAfterTraining();
    hiddenBias->cleanUpAfterTraining();
}

float RProp::predict(
        const Board& board,
        const std::map<Block*, BlockFinalFeatures>& featureMap) const
{
////
    return 0.0;
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

    if(!inputLayer->writeToFile(f) ||
       !inputBias->writeToFile(f) ||
       !hiddenLayer->writeToFile(f) ||
       !hiddenBias->writeToFile(f))
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

    if(!inputLayer->readFromFile(f) ||
       !inputBias->readFromFile(f) ||
       !hiddenLayer->readFromFile(f) ||
       !hiddenBias->readFromFile(f))
    {
        fclose(f);

        return false;
    }

    fclose(f);

    return true;
}

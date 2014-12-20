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

float RProp::calculateR(const float* features) const
{
    int inputSize = inputLayer->getWidth();
    int hiddenSize = hiddenLayer->getHeight();

    float inputTemp[inputSize];

    for(int i = 0; i < inputSize; ++i)
    {
        inputTemp[i] = features[i] + inputBias->getWeight(i);
    }

    float result = 0.0;

    for(int i = 0; i < hiddenSize; ++i)
    {
        float sum = 0.0;

        for(int j = 0; j < inputSize; ++j)
        {
            sum += inputLayer->getWeight(i, j) * inputTemp[j];
        }

        result += hiddenLayer->getWeight(i) * (sum + hiddenBias->getWeight(i));
    }

    return result;
}

float RProp::predict(
        const Board& board0,
        const std::map<Block*, BlockFinalFeatures>& featureMap) const
{
    Board board = board0;

    std::set<Block*> blocks;
    float result = board.getScore();

    board.getBlocks(blocks);

    std::set<Block*>::iterator itt = blocks.begin();
    std::set<Block*>::iterator end = blocks.end();

    for( ; itt != end; ++itt)
    {
        if((*itt)->getState() != EMPTY)
        {
            float* features = getFeatureVector(featureMap.find(*itt)->second);

            float r = calculateR(features);

            delete[] features;

            if(r < 0.0)
            {
                if((*itt)->getState() == BLACK)
                {
                    result += (*itt)->getSize();
                }
                else
                {
                    result -= (*itt)->getSize();
                }

                (*itt)->setState(EMPTY);
            }
        }
    }

    board.splitEmptyBlocks();

    board.getBlocks(blocks);

    itt = blocks.begin();
    end = blocks.end();

    for( ; itt != end; ++itt)
    {
        if((*itt)->getState() == EMPTY)
        {
            SpaceState adjacent = EMPTY;
            bool disputed = false;

            std::set<Block*> adjacentBlocks;

            board.getAdjacentBlocks(adjacentBlocks, *itt);

            std::set<Block*>::iterator blockItt = adjacentBlocks.begin();
            std::set<Block*>::iterator blockEnd = adjacentBlocks.end();

            for( ; blockItt != blockEnd; ++blockItt)
            {
                if((*blockItt)->getState() == BLACK)
                {
                    if(adjacent == WHITE)
                    {
                        disputed = true;

                        break;
                    }
                    else
                    {
                        adjacent = BLACK;
                    }
                }
                else
                {
                    if(adjacent == BLACK)
                    {
                        disputed = true;

                        break;
                    }
                    else
                    {
                        adjacent = WHITE;
                    }
                }
            }

            if(!disputed)
            {
                if(adjacent == BLACK)
                {
                    result -= (*itt)->getSize();
                }
                else
                {
                    result += (*itt)->getSize();
                }
            }
        }
    }

    return result;
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

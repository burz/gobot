#include "bootstrap.h"
#include "directoryLoader.h"
#include "lifeFile.h"

#include <cstdio>

#define STARTING_LABELS 10
#define LABEL_STEP 5

namespace {
const char empty[] = "E";
const char blackUndetermined[] = "\e[34mB\e[0m";
const char whiteUndetermined[] = "\e[93mW\e[0m";
const char blackAlive[] = "\e[34mA\e[0m";
const char whiteAlive[] = "\e[93mA\e[0m";
const char blackDead[] = "\e[34mD\e[0m";
const char whiteDead[] = "\e[93mD\e[0m";
const char blackSelected[] = "\e[91mB\e[0m";
const char whiteSelected[] = "\e[91mW\e[0m";
}

Bootstrap::Bootstrap(
        const char* _sourceDirectory,
        const char* _destinationDirectory,
        const char* _labelDirectory)
{
    sourceDirectory = _sourceDirectory;
    destinationDirectory = _destinationDirectory;
    labelDirectory = _labelDirectory;
}

void Bootstrap::printBoard(
        const Board& board,
        std::map<Block*, bool>& lifeMap,
        Block* selectedBlock) const
{
    int size = board.getSize();

    for(int i = 0; i < size; ++i)
    {
        for(int j = 0; j < size; ++j)
        {
            const char* string = empty;

            Block* block = board.getBlock(i, j);

            if(block == selectedBlock)
            {
                if(block->getState() == BLACK)
                {
                    string = blackSelected;
                }
                else
                {
                    string = whiteSelected;
                }
            }
            else if(block->getState() != EMPTY)
            {
                std::map<Block*, bool>::iterator mapping = lifeMap.find(block);

                if(block->getState() == BLACK)
                {
                    if(mapping != lifeMap.end())
                    {
                        if(mapping->second)
                        {
                            string = blackAlive;
                        }
                        else
                        {
                            string = blackDead;
                        }
                    }
                    else
                    {
                        string = blackUndetermined;
                    }
                }
                else
                {
                    if(mapping != lifeMap.end())
                    {
                        if(mapping->second)
                        {
                            string = whiteAlive;
                        }
                        else
                        {
                            string = whiteDead;
                        }
                    }
                    else
                    {
                        string = whiteUndetermined;
                    }
                }
            }

            printf("%s, ", string);
        }

        printf("\b\b \n");
    }
}

bool Bootstrap::manuallyLabelBoard(const char* boardFile) const
{
    Board board;
    std::map<Block*, BlockFinalFeatures> featureMap;
    char buffer[100];

    sprintf(buffer, "%s/%s", sourceDirectory, boardFile);

    if(!board.readFromFile(buffer, featureMap))
    {
        printf("Couldn't read board file: %s\n", buffer);

        return false;
    }

    printf("\n$$$$$$$$$$$$$$$ MANUAL LABELING $$$$$$$$$$$$$$$\n\n");

    board.print();

    printf("Is the board malformed (y/n)? ");

    char response = getchar();

    getchar();

    if(response == 'y')
    {
        return false;
    }

    std::map<Block*, bool> lifeMap;
    std::set<Block*> blocks;

    board.getBlocks(blocks);

    std::set<Block*>::iterator itt = blocks.begin();
    std::set<Block*>::iterator end = blocks.end();

    for( ; itt != end; ++itt)
    {
        if((*itt)->getState() != EMPTY)
        {
            printf("\n=====================================\n\n");

            printBoard(board, lifeMap, *itt);

            printf("What is the state of the block (a/d)? ");

            response = getchar();

            getchar();

            bool alive = response == 'a';

            std::pair<Block*, bool> mapping(*itt, alive);

            lifeMap.insert(mapping);
        }
    }

    sprintf(buffer, "%s/%sl", labelDirectory, boardFile);

    if(!writeLifeFile(lifeMap, buffer))
    {
        printf("Could not write the life map to %s\n", buffer);

        return false;
    }

    float calculatedScore = board.calculateFinalScore(lifeMap);

    printf("\n$$$$$$$$$$$$$$$ FINAL BOARD $$$$$$$$$$$$$$$\n\n");

    board.print();

    printf("Calculated Score: %f -- Final Score: %f\n",
           calculatedScore, board.getFinalScore());

    printf("\nIs this score correct (y/n)? ");

    bool result = getchar();

    getchar();

    if(result == 'y')
    {
        return true;
    }
    else
    {
        return manuallyLabelBoard(boardFile);
    }
}

bool Bootstrap::automaticallyLabelBoard(const RProp& model, const char* boardFile) const
{
    char buffer[100];

    sprintf(buffer, "%s/%s", sourceDirectory, boardFile);

    Board board;
    std::map<Block*, BlockFinalFeatures> featureMap;

    if(!board.readFromFile(buffer, featureMap))
    {
        printf("Couldn't read board from file %s\n", buffer);

        return false;
    }

    std::map<Block*, bool> lifeMap;

    std::map<Block*, BlockFinalFeatures>::iterator itt = featureMap.begin();
    std::map<Block*, BlockFinalFeatures>::iterator end = featureMap.end();

    for( ; itt != end; ++itt)
    {
        float* features = getFeatureVector(itt->second);

        bool alive = model.calculateR(features) >= 0.0;

        delete[] features;

        std::pair<Block*, bool> mapping(itt->first, alive);

        lifeMap.insert(mapping);
    }

    sprintf(buffer, "%s/%sl", labelDirectory, boardFile);

    if(!writeLifeFile(lifeMap, buffer))
    {
        printf("Could not write the life map to %s\n", buffer);

        return false;
    }

    float score = board.calculateFinalScore(lifeMap);

    if(score != board.getFinalScore())
    {
        return false;
    }

    return true;
}

inline
void handleTouchedFiles(
        std::vector<const char*>& toMove,
        std::vector<const char*>& toRemove,
        const char* sourceDirectory,
        const char* destinationDirectory)
{
    char buffer[100];
    char destBuffer[100];

    std::vector<const char*>::iterator itt = toMove.begin();
    std::vector<const char*>::iterator end = toMove.end();

    for( ; itt != end; ++itt)
    {
        sprintf(buffer, "%s/%s", sourceDirectory, *itt);
        sprintf(destBuffer, "%s/%s", destinationDirectory, *itt);

        if(!rename(buffer, destBuffer))
        {
            printf("Couldn't rename %s to %s\n", buffer, destBuffer);
        }
    }

    itt = toRemove.begin();
    end = toRemove.end();

    for( ; itt != end; ++itt)
    {
        sprintf(buffer, "%s/%s", sourceDirectory, *itt);

        if(!remove(buffer))
        {
            printf("Couldn't remove file %s\n", buffer);
        }
    }
}

void Bootstrap::run(RProp& model) const
{
    int numberOfBoards = numberOfFilesIn(sourceDirectory);

    if(!numberOfBoards)
    {
        printf("No files in %s\n", sourceDirectory);

        return;
    }

    int startingSize = STARTING_LABELS < numberOfBoards ?
          STARTING_LABELS
        : numberOfBoards;

    std::vector<const char*> toMove;
    std::vector<const char*> toRemove;

    DirectoryIterator itt(sourceDirectory);
    DirectoryIterator end = DirectoryIterator::end();

    for(int i = 0; i < startingSize; ++i)
    {
        if(manuallyLabelBoard(*itt))
        {
            toMove.push_back(*itt);
        }
        else
        {
            toRemove.push_back(*itt);
        }

        ++itt;
    }

    handleTouchedFiles(toMove, toRemove, sourceDirectory, destinationDirectory);

    while(numberOfFilesIn(sourceDirectory) > 0)
    {
        bool changed = false;

        toMove.clear();
        toRemove.clear();

        DirectoryIterator sourceFiles(sourceDirectory);

        for( ; sourceFiles != end; ++sourceFiles)
        {
            if(automaticallyLabelBoard(model, *sourceFiles))
            {
                toMove.push_back(*sourceFiles);

                changed = true;
            }
        }

        if(!changed)
        {
            numberOfBoards = numberOfFilesIn(sourceDirectory);

            int stepSize = LABEL_STEP > numberOfBoards ?
                  numberOfBoards
                : LABEL_STEP;

            DirectoryIterator boardFiles(sourceDirectory);

            for(int i = 0; i < stepSize; ++i)
            {
                if(manuallyLabelBoard(*boardFiles))
                {
                    toMove.push_back(*boardFiles);
                }
                else
                {
                    toRemove.push_back(*boardFiles);
                }

                ++boardFiles;
            }
        }

        handleTouchedFiles(toMove, toRemove, sourceDirectory, destinationDirectory);
    }
}

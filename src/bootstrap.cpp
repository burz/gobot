#include "bootstrap.h"
#include "directoryLoader.h"

#include <cstdio>

#define STARTING_LABELS 1
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

void Bootstrap::manuallyLabelBoard(const char* boardFile) const
{
    Board board;
    std::map<Block*, BlockFinalFeatures> featureMap;
    char buffer[100];

    sprintf(buffer, "%s/%s", sourceDirectory, boardFile);

    if(!board.readFromFile(buffer, featureMap))
    {
        printf("Couldn't read board file: %s\n", boardFile);

        return;
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

    DirectoryIterator itt(sourceDirectory);
    DirectoryIterator end = DirectoryIterator::end();

    for(int i = 0; i < startingSize; ++i)
    {
        manuallyLabelBoard(*itt);

        ++itt;
    }

    return;

    while(numberOfFilesIn(sourceDirectory) > 0)
    {
        DirectoryIterator sourceFiles(sourceDirectory);

        for( ; sourceFiles != end; ++sourceFiles)
        {
// TODO
        }
    }
}

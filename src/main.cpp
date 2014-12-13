#include "block.h"
#include "board.h"
#include "game.h"
#include "parser.h"
#include "directoryLoader.h"
#include "featureFile.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
    char buffer[100];

    DirectoryIterator itt("reformatedGames");
    DirectoryIterator end = DirectoryIterator::end();

    for( ; itt != end; ++itt)
    {
        sprintf(buffer, "reformatedGames/%s", *itt);

        Game game;

        if(!parseFile(&game, buffer))
        {
            continue;
        }

        Board finalBoard = game.playGame();

        sprintf(buffer, "gameFeatures/%sf", *itt);

        if(!writeFeaturesToFile(finalBoard, buffer))
        {
            continue;
        }

        std::map<BoardLocation, BlockFinalFeatures> featureMap;

        if(!readFeaturesFromFile(featureMap, buffer))
        {
            continue;
        }

        std::pair<BoardLocation, BlockFinalFeatures> mapping = *featureMap.begin();

        std::set<Block*> blocks;

        finalBoard.getBlocks(blocks);

        bool equal = false;

        std::set<Block*>::const_iterator blockItt = blocks.begin();
        std::set<Block*>::const_iterator blockEnd = blocks.end();

        for( ; blockItt != blockEnd; ++blockItt)
        {
            if((*blockItt)->getState() != EMPTY)
            {
                if((*blockItt)->contains(mapping.first))
                {
                    if(mapping.second.size == (*blockItt)->getSize() &&
                       mapping.second.liberties == (*blockItt)->getLiberties())
//                    if(mapping.second == finalBoard.generateFinalFeatures(*blockItt))
                    {
                        equal = true;

                        break;
                    }
                }
            }
        }

        printf("%d\n", equal);
    }

//    std::vector<Game> games;
//
//    loadDirectory(games, "reformatedGames");
//
//    int i = 0;
//
//    std::vector<Game>::iterator itt = games.begin();
//    std::vector<Game>::iterator end = games.end();
//
//    for( ; itt != end; ++itt)
//    {
//        itt->generateFeatureVectors();
//
//        if(i % 100 == 0)
//        {
//            printf(".");
//            fflush(stdout);
//        }
//
//        ++i;
//    }
//
//    printf("\n");

    return 0;
}

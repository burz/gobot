#include "block.h"
#include "board.h"
#include "game.h"
#include "parser.h"
#include "directoryLoader.h"
#include "featureFile.h"
#include "rprop.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
    RProp rprop;

    std::vector<Game> games;

    rprop.train(games, 4);

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

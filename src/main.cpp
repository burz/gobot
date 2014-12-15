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
    DirectoryIterator games("gamesReformated");

    RProp model(NUMBER_OF_FEATURES, 5);

    model.trainWithFeatures(games, "gamesFeatures");

    model.outputToFile("rprop_1_5.model");

//    std::vector<Game> games;
//
//    if(loadDirectory(games, "gamesReformated"))
//    {
//        RProp model(NUMBER_OF_FEATURES, 13);
//
//        model.train(games, 5);
//    }

    return 0;
}

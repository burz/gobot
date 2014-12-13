#include "block.h"
#include "board.h"
#include "game.h"
#include "parser.h"
#include "directoryLoader.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
    DirectoryIterator itt("reformatedGames");
    DirectoryIterator end = DirectoryIterator::end();

    for( ; itt != end; ++itt)
    {
        Game game = *itt;

        Board board = game.playGame();
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

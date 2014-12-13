#include "board.h"
#include "game.h"
#include "parser.h"
#include "directoryLoader.h"
#include "featureFile.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("Usage generateFeatureFiles inputDirectory outputDirectly\n");

        return 1;
    }

    char buffer[100];
    int i = 0;

    DirectoryIterator itt(argv[1]);
    DirectoryIterator end = DirectoryIterator::end();

    for( ; itt != end; ++itt)
    {
        sprintf(buffer, "%s/%s", argv[1], *itt);

        Game game;

        if(!parseFile(&game, buffer))
        {
            printf("Couldn't parse %s\n", buffer);

            return 1;
        }

        Board finalBoard = game.playGame();

        sprintf(buffer, "%s/%sf", argv[2], *itt);

        if(!writeFeaturesToFile(finalBoard, buffer))
        {
            printf("Couldn't write the features of %s to a file\n", buffer);

            return 1;
        }

        if(i % 100 == 0)
        {
            printf(".");

            fflush(stdout);
        }

        ++i;
    }

    printf("\n");

    return 0;
}

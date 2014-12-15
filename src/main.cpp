#include "block.h"
#include "board.h"
#include "game.h"
#include "parser.h"
#include "directoryLoader.h"
#include "featureFile.h"
#include "rprop.h"

#include <stdio.h>

#define HIDDEN_LAYER_SIZE 5

const char usage[] = "Usage: gobot -train modelOutputFile gameDirectory "
                     "[featureDirectory]\n"
                     "             -test modelFile gameDirectory [featureDirectory]\n"
                     "             -predict modelFile gameFile [featureFile]\n"
                     "             -generateFeatures gameDirectory featureDirectory";

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("%s\n", usage);

        return 1;
    }
    else if(!strcmp(argv[1], "-train"))
    {
        if(argc == 3)
        {
            printf("%s\n", usage);

            return 1;
        }

        DirectoryIterator games(argv[3]);

        RProp model(NUMBER_OF_FEATURES, HIDDEN_LAYER_SIZE);

        if(argc > 4)
        {
            model.trainWithFeatures(games, argv[4]);
        }
        else
        {
            model.train(games);
        }

        model.outputToFile(argv[2]);
    }

    return 0;
}

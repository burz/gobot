#include "block.h"
#include "board.h"
#include "game.h"
#include "parser.h"
#include "directoryLoader.h"
#include "rprop.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>

#define HIDDEN_LAYER_SIZE 20

const char usage[] = "Usage: gobot "
//                     "-train modelOutputFile iterations gameDirectory [featureDirectory]\n"
//                     "             -test modelFile gameDirectory [featureDirectory]\n"
//                     "             -predict modelFile gameFile [featureFile]\n"
                     "             -generateBoards gameDirectory boardDirectory";

int main(int argc, char *argv[])
{
    if(argc < 4)
    {
        printf("%s\n", usage);

        return 1;
    }
    else if(!strcmp(argv[1], "-generateBoards"))
    {
        char buffer[100];
        int i = 0;
        DirectoryIterator games(argv[2]);

        for( ; games != games.end(); ++games)
        {
            Game game;

            sprintf(buffer, "%s/%s", argv[2], *games);

            if(!parseFile(&game, buffer))
            {
                printf("\nERROR: Couldn't read game file: %s\n", buffer);

                return 1;
            }

            Board board = game.playGame();

            sprintf(buffer, "%s/%sb", argv[3], *games);

            if(!board.writeToFile(buffer))
            {
                printf("\nERROR: Could not write the board to: %s\n", buffer);

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
    }
//    else if(!strcmp(argv[1], "-train"))
//    {
//        if(argc < 5)
//        {
//            printf("%s\n", usage);
//
//            return 1;
//        }
//
//        int iterations = atoi(argv[3]);
//
//        if(iterations < 1)
//        {
//            iterations = 1;
//        }
//
//        DirectoryIterator games(argv[4], iterations);
//
//        RProp model(NUMBER_OF_FEATURES, HIDDEN_LAYER_SIZE);
//
//        if(argc > 5)
//        {
//            model.trainWithFeatures(games, argv[5]);
//        }
//        else
//        {
//            model.train(games);
//        }
//
//        model.outputToFile(argv[2]);
//
//        model.print();
//    }
//    else if(!strcmp(argv[1], "-test"))
//    {
//        DirectoryIterator games(argv[3]);
//
//        RProp model;
//
//        if(!model.readFromFile(argv[2]))
//        {
//            printf("Couldn't read model from: %s\n", argv[2]);
//
//            return 1;
//        }
//
//        if(argc > 4)
//        {
//            printf("Accuracy: %f%%\n", model.testWithFeatures(games, argv[4]));
//        }
//        else
//        {
//            printf("Accuracy: %f%%\n", model.test(games));
//        }
//    }
//    else if(!strcmp(argv[1], "-predict"))
//    {
//        Game game;
//
//        if(!parseFile(&game, argv[3]))
//        {
//            printf("Couldn't read game file: %s\n", argv[3]);
//
//            return 1;
//        }
//
//        RProp model;
//
//        if(!model.readFromFile(argv[2]))
//        {
//            printf("Couldn't read model from: %s\n", argv[2]);
//
//            return 1;
//        }
//
//        if(argc > 4)
//        {
//            std::map<BoardLocation, BlockFinalFeatures> featureMap;
//
//            if(!readFeaturesFromFile(featureMap, argv[4]))
//            {
//                printf("Couldn't read feature file: %s\n", argv[4]);
//
//                return 1;
//            }
//
//            printf("Estimate: %f\n", model.predictWithFeatures(game, featureMap));
//        }
//        else
//        {
//            printf("Estimate: %f\n", model.predict(game));
//        }
//    }
    else
    {
        printf("%s\n", usage);

        return 1;
    }

    return 0;
}

#include "parser.h"

#include <cstdio>

bool parseFile(Game* game, const char *filename, const int size)
{
    FILE *file = fopen(filename, "r");

    if(file == 0)
    {
        return false;
    }

    float komi;
    float finalScore;
    int handicapStones;

    if(fscanf(file, "%f %f %i", &komi, &finalScore, &handicapStones) == EOF)
    {
        fclose(file);

        return false;
    }

    Game result(size, komi, finalScore);

    int x;
    int y;

    for(int i = 0; i < handicapStones; ++i)
    {
        if(fscanf(file, "%i %i", &x, &y) == EOF)
        {
            fclose(file);

            return false;
        }

        BoardLocation location(x, y);

        result.addHandicap(location);
    }

    while(fscanf(file, "%i %i", &x, &y) != EOF)
    {
        BoardLocation location(x, y);

        result.addMove(location);
    }

    *game = result;

    fclose(file);

    return true;
}

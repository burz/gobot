#include "parser.h"

#include <stdio.h>

bool parseFile(Game* game, const char *filename, const int size)
{
    FILE *file = fopen(filename, "r");

    if(file == 0)
    {
        return false;
    }

    float komi;
    float finalScore;

    if(fscanf(file, "%f %f", &komi, &finalScore) == EOF)
    {
        fclose(file);

        return false;
    }

    Game result(size, komi, finalScore);

    int x;
    int y;

    while(fscanf(file, "%i %i", &x, &y) != EOF)
    {
        BoardLocation location;

        location.x = x;
        location.y = y;

        result.addMove(location);
    }

    *game = result;

    fclose(file);

    return true;
}

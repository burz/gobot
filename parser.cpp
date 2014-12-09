#include "parser.h"

#include <stdio.h>

Game::Game(void)
{
    komi = 0.0;
    finalScore = 0.0;
}

Game::Game(const float _komi, const float _finalScore)
{
    komi = _komi;
    finalScore = _finalScore;
}

bool parseFile(Game* game, const char *filename)
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

    Game result(komi, finalScore);

    int x;
    int y;

    while(fscanf(file, "%i %i", &x, &y) != EOF)
    {
        BoardLocation location;

        location.x = x;
        location.y = y;

        result.moves.push_back(location);
    }

    *game = result;

    fclose(file);

    return true;
}

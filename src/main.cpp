#include "block.h"
#include "board.h"
#include "game.h"
#include "parser.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
    Game game;

    bool result = parseFile(&game, "output.sgfo");

    if(result)
    {
        game.playGame();
    }

    return 0;
}

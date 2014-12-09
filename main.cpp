#include "board.h"
#include "block.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
    Board board(19, 6.5);

    board.print();
    printf("\n========================\n\n");

    board.playMove(0, 0, BLACK);

    board.print();
    printf("\n========================\n\n");

    board.playMove(0, 2, WHITE);

    board.print();
    printf("\n========================\n\n");

    board.playMove(0, 1, BLACK);

    board.print();

    return 0;
}

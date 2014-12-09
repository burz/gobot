#include "board.h"
#include "block.h"
#include "parser.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
    Board board(19, 6.5);

    board.print();

    printf("\n========================\n\n");
    board.playMove(0, 0, BLACK);

    board.print();

    Block* block = board.getBlock(0, 0);
    block->print();
    printf("size: %d, liberties: %d\n", block->getSize(), block->getLiberties());

    printf("\n========================\n\n");
    board.playMove(0, 2, WHITE);

    board.print();

    block = board.getBlock(0, 0);
    block->print();
    printf("size: %d, liberties: %d\n", block->getSize(), block->getLiberties());

    printf("+++++++++++++++++++++++++++\n");

    block = board.getBlock(0, 2);
    block->print();
    printf("size: %d, liberties: %d\n", block->getSize(), block->getLiberties());

    printf("\n========================\n\n");
    board.playMove(0, 1, BLACK);

    board.print();

    block = board.getBlock(0, 1);
    block->print();
    printf("size: %d, liberties: %d\n", block->getSize(), block->getLiberties());

    printf("+++++++++++++++++++++++++++\n");

    block = board.getBlock(0, 2);
    block->print();
    printf("size: %d, liberties: %d\n", block->getSize(), block->getLiberties());

    printf("\n========================\n\n");
    board.playMove(1, 2, WHITE);

    board.print();

    block = board.getBlock(0, 1);
    block->print();
    printf("size: %d, liberties: %d\n", block->getSize(), block->getLiberties());

    printf("+++++++++++++++++++++++++++\n");

    block = board.getBlock(0, 2);
    block->print();
    printf("size: %d, liberties: %d\n", block->getSize(), block->getLiberties());

    printf("\n========================\n\n");
    board.playMove(1, 1, BLACK);

    board.print();

    block = board.getBlock(0, 1);
    block->print();
    printf("size: %d, liberties: %d\n", block->getSize(), block->getLiberties());

    printf("+++++++++++++++++++++++++++\n");

    block = board.getBlock(0, 2);
    block->print();
    printf("size: %d, liberties: %d\n", block->getSize(), block->getLiberties());

    printf("\n========================\n\n");
    board.playMove(1, 3, WHITE);

    board.print();

    block = board.getBlock(0, 1);
    block->print();
    printf("size: %d, liberties: %d\n", block->getSize(), block->getLiberties());

    printf("+++++++++++++++++++++++++++\n");

    block = board.getBlock(0, 2);
    block->print();
    printf("size: %d, liberties: %d\n", block->getSize(), block->getLiberties());

    Game game;
    bool result = parseFile(&game, "output.sgfo");

    printf("%d\n", result);

    return 0;
}

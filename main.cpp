#include "board.h"
#include "block.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
    Board board(19, 6.5);

    board.print();

    BoardLocation l0;
    BoardLocation l1;

    l0.x = 0;
    l0.y = 1;
    l1.x = 0;
    l1.y = 0;

    SpaceState ss = BLACK;

    Block b0(ss, l0, 3);
    Block b1(ss, l1, 2);

    b0.absorb(&b1);

    printf("%d\n", b0.getLiberties());

    b0.print();

    return 0;
}

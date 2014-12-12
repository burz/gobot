#include "game.h"
#include "board.h"

Game::Game(void)
{
    size = 19;
    komi = 0.0;
    finalScore = 0.0;
}

Game::Game(const int _size, const float _komi, const float _finalScore)
{
    size = _size;
    komi = _komi;
    finalScore = _finalScore;
}

void Game::addMove(const BoardLocation location)
{
    moves.push_back(location);
}

void Game::playGame(bool printTurns) const
{
    Board board(size, komi);

    std::vector<BoardLocation>::const_iterator itt = moves.begin();

    int i = 1;

    for( ; itt != moves.end(); ++itt)
    {
        SpaceState state = i % 2 == 1 ? BLACK : WHITE;

        board.playMove(itt->x, itt->y, state);

        if(printTurns)
        {
            printf("\n========Turn: %3d========\n\nScore: %f\nMove: (%d, %d)\n\n",
               i,
               board.getScore(),
               itt->x,
               itt->y);

            board.print();

            getchar();
        }

        ++i;
    }

    printf("\n!!!!!!!!!!!!!FINAL BOARD!!!!!!!!!!!!!\n\n");

    board.print();

    Block* block = board.getBlock(0, 1);
    board.generateFinalFeatures(block).print();

    printf("\n##########################\n\n");

    block = board.getBlock(18, 13);
    board.generateFinalFeatures(block).print();

    printf("\n##########################\n\n");

    block = board.getBlock(6, 13);
    board.generateFinalFeatures(block).print();

    printf("\n##########################\n\n");

    block = board.getBlock(0, 13);
    board.generateFinalFeatures(block).print();
}

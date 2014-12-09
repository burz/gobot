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

void Game::playGame(void) const
{
    Board board(size, komi);

    std::vector<BoardLocation>::const_iterator itt = moves.begin();

    int i = 1;

    for( ; itt != moves.end(); ++itt)
    {
        printf("\n========Move: %4d========\n\nScore: %f\n\n", i, board.getScore());

        SpaceState state = i % 2 == 1 ? BLACK : WHITE;

        board.playMove(itt->x, itt->y, state);

        board.print();

        getchar();

        ++i;
    }
}

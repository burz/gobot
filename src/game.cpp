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

Game::~Game()
{
    std::map<Block*, float*>::iterator itt = featureMap.begin();
    std::map<Block*, float*>::iterator end = featureMap.end();

    for( ; itt != end; ++itt)
    {
        delete[] itt->second;
    }
}

void Game::addMove(const BoardLocation location)
{
    moves.push_back(location);
}

Board Game::playGame(void) const
{
    Board board(size, komi);

    std::vector<BoardLocation>::const_iterator itt = moves.begin();

    int i = 1;

    for( ; itt != moves.end(); ++itt)
    {
        SpaceState state = i % 2 == 1 ? BLACK : WHITE;

        if(!(itt->x == 19 && itt->y == 19))
        {
            board.playMove(itt->x, itt->y, state);
        }

        ++i;
    }

    board.splitEmptyBlocks();

    return board;
}

void Game::generateFeatureVectors(void)
{
    Board board(size, komi);

    std::vector<BoardLocation>::const_iterator itt = moves.begin();
    std::vector<BoardLocation>::const_iterator end = moves.end();

    int i = 1;

    for( ; itt != end; ++itt)
    {
        SpaceState state = i % 2 == 1 ? BLACK : WHITE;

        if(!(itt->x == 19 && itt->y == 19))
        {
            board.playMove(itt->x, itt->y, state);
        }

        ++i;
    }

    board.splitEmptyBlocks();

    std::set<Block*> blocks;

    board.getBlocks(blocks);

    std::set<Block*>::iterator blockItt = blocks.begin();
    std::set<Block*>::iterator blockEnd = blocks.end();

    for( ; blockItt != blockEnd; ++blockItt)
    {
        if((*blockItt)->getState() != EMPTY)
        {
            float* features = board.generateFinalFeatureVector(*blockItt);

            std::pair<Block*, float*> mapping(*blockItt, features);

            featureMap.insert(mapping);
        }
    }
}

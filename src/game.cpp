#include "game.h"
#include "board.h"

#include <cstdio>

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

float Game::getKomi(void) const
{
    return komi;
}

float Game::getFinalScore(void) const
{
    return finalScore;
}

void Game::addHandicap(const BoardLocation& location)
{
    handicap.push_back(location);
}

void Game::addMove(const BoardLocation& location)
{
    moves.push_back(location);
}

Board Game::playGame(void) const
{
    Board board(size, komi, finalScore);

    int i = 1;

    if(handicap.size() > 0)
    {
        std::vector<BoardLocation>::const_iterator handicapItt = handicap.begin();
        std::vector<BoardLocation>::const_iterator handicapEnd = handicap.end();

        for( ; handicapItt != handicapEnd; ++handicapItt)
        {
            board.playMove(handicapItt->x, handicapItt->y, BLACK);
        }

        ++i;
    }

    std::vector<BoardLocation>::const_iterator itt = moves.begin();
    std::vector<BoardLocation>::const_iterator end = moves.end();

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

bool Game::parseFile(const char *filename, const int _size)
{
    FILE *file = fopen(filename, "r");

    if(file == 0)
    {
        return false;
    }

    size = _size;

    int handicapStones;

    if(fscanf(file, "%f %f %i", &komi, &finalScore, &handicapStones) == EOF)
    {
        fclose(file);

        return false;
    }

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

        handicap.push_back(location);
    }

    while(fscanf(file, "%i %i", &x, &y) != EOF)
    {
        BoardLocation location(x, y);

        moves.push_back(location);
    }

    fclose(file);

    return true;
}

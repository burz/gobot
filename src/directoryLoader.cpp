#include "directoryLoader.h"
#include "parser.h"

#include <stdio.h>
#include <dirent.h>

bool loadDirectory(std::vector<Game>& games, const char* directory)
{
    DIR* dir = opendir(directory);

    if(dir == 0)
    {
        printf("Could not open directory: %s\n", directory);

        return false;
    }

    struct dirent *ent;

    while((ent = readdir(dir)) != 0)
    {
        char filePath[100];

        sprintf(filePath, "%s/%s", directory, ent->d_name);

        Game game;

        parseFile(&game, filePath);

        games.push_back(game);
    }

    closedir(dir);

    return true;
}

DirectoryIterator::DirectoryIterator(void)
{
    dir = 0;
}

DirectoryIterator::DirectoryIterator(const char* _directory)
{
    strcpy(directory, _directory);

    dir = opendir(_directory);

    if(dir == 0)
    {
        printf("Could not open directory: %s\n", directory);
    }

    ent = readdir(dir);

    if(!ent)
    {
        closedir(dir);

        dir = 0;
    }
}

DirectoryIterator::~DirectoryIterator()
{
    if(dir)
    {
        closedir(dir);
    }
}

Game DirectoryIterator::operator*(void)
{
    Game game;

    if(dir && ent)
    {
        char filePath[100] = "";

        sprintf(filePath, "%s/%s", directory, ent->d_name);

        parseFile(&game, filePath);
    }

    return game;
}

DirectoryIterator& DirectoryIterator::operator++(void)
{
    if(dir && ent)
    {
        ent = readdir(dir);
    }

    if(!ent)
    {
        closedir(dir);

        dir = 0;
    }

    return *this;
}

DirectoryIterator DirectoryIterator::end(void)
{
    DirectoryIterator result;

    return result;
}

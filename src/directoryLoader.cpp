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

DirectoryIterator::DirectoryIterator(const char* directory)
{
    dir = opendir(directory);

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

const char* DirectoryIterator::operator*(void)
{
    if(dir && ent)
    {
        return ent->d_name;
    }
    else
    {
        return 0;
    }
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

#include "directoryLoader.h"
#include "gameParser.h"

#include <cstdio>
#include <cstring>

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
        if(!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
        {
            continue;
        }

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

DirectoryIterator::DirectoryIterator(const char* _directory, const int& _maxLoops)
{
    directory = _directory;
    maxLoops = _maxLoops;
    currentLoop = 0;

    dir = opendir(directory);

    if(dir == 0)
    {
        printf("Could not open directory: %s\n", directory);

        ent = 0;

        return;
    }

    ent = readdir(dir);

    if(ent && !strcmp(ent->d_name, "."))
    {
        ent = readdir(dir);
    }
    if(ent && !strcmp(ent->d_name, ".."))
    {
        ent = readdir(dir);
    }
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

const char* DirectoryIterator::getDirectory(void) const
{
    return directory;
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

        ++currentLoop;

        if(currentLoop < maxLoops)
        {
            dir = opendir(directory);

            if(dir == 0)
            {
                printf("Could not open directory: %s\n", directory);
            }

            ent = readdir(dir);

            if(ent && !strcmp(ent->d_name, "."))
            {
                ent = readdir(dir);
            }
            if(ent && !strcmp(ent->d_name, ".."))
            {
                ent = readdir(dir);
            }
            if(!ent)
            {
                closedir(dir);

                dir = 0;
            }
        }
        else
        {
            dir = 0;
        }
    }

    return *this;
}

DirectoryIterator DirectoryIterator::end(void)
{
    DirectoryIterator result;

    return result;
}

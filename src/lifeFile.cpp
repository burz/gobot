#include "lifeFile.h"

#include <cstdio>

bool writeLifeFile(std::map<Block*, bool>& lifeMap, const char* filename)
{
    FILE* f = fopen(filename, "wb");

    if(!f)
    {
        return false;
    }

    int numberOfBlocks = lifeMap.size();

    if(fwrite(&numberOfBlocks, sizeof(int), 1, f) != 1)
    {
        fclose(f);

        return false;
    }

    std::map<Block*, bool>::iterator itt = lifeMap.begin();
    std::map<Block*, bool>::iterator end = lifeMap.end();

    for( ; itt != end; ++end)
    {
        BoardLocation location = *itt->first->locationsBegin();

        if(fwrite(&location.x, sizeof(int), 1, f) != 1 ||
           fwrite(&location.y, sizeof(int), 1, f) != 1 ||
           fwrite(&itt->second, sizeof(bool), 1, f) != 1)
        {
            fclose(f);

            return false;
        }
    }

    fclose(f);

    return false;
}

bool readLifeFile(std::map<BoardLocation, bool>& lifeMap, const char* filename)
{
    FILE* f = fopen(filename, "rb");

    if(!f)
    {
        return false;
    }

    int numberOfBlocks = lifeMap.size();

    if(fread(&numberOfBlocks, sizeof(int), 1, f) != 1)
    {
        fclose(f);

        return false;
    }

    for(int i = 0; i < numberOfBlocks; ++i)
    {
        BoardLocation location(0, 0);
        bool alive;

        if(fread(&location.x, sizeof(int), 1, f) != 1 ||
           fread(&location.y, sizeof(int), 1, f) != 1 ||
           fread(&alive, sizeof(bool), 1, f) != 1)
        {
            fclose(f);

            return false;
        }

        std::pair<BoardLocation, bool> mapping(location, alive);

        lifeMap.insert(mapping);
    }

    fclose(f);

    return false;
}

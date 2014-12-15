#include "featureFile.h"

bool writeFeaturesToFile(Board& board, const char* destination)
{
    FILE* f = fopen(destination, "wb");

    if(!f)
    {
        return false;
    }

    std::set<Block*> blocks;

    board.getBlocks(blocks);

    std::set<Block*> nonEmptyBlocks;

    std::set<Block*>::iterator itt = blocks.begin();
    std::set<Block*>::iterator end = blocks.end();

    for( ; itt != end; ++itt)
    {
        if((*itt)->getState() != EMPTY)
        {
            nonEmptyBlocks.insert(*itt);
        }
    }

    int size = nonEmptyBlocks.size();

    if(fwrite(&size, sizeof(int), 1, f) != 1)
    {
        fclose(f);

        return false;
    }

    itt = nonEmptyBlocks.begin();
    end = nonEmptyBlocks.end();

    for( ; itt != end; ++itt)
    {
        BoardLocation location = *(*itt)->locationsBegin();

        if(fwrite(&location, sizeof(BoardLocation), 1, f) != 1)
        {
            fclose(f);

            return false;
        }

        BlockFinalFeatures features = board.generateFinalFeatures(*itt);

        if(fwrite(&features, sizeof(BlockFinalFeatures), 1, f) != 1)
        {
            fclose(f);

            return false;
        }
    }

    fclose(f);

    return true;
}

bool readFeaturesFromFile(
        std::map<BoardLocation, BlockFinalFeatures>& featureMap,
        const char* file)
{
    FILE *f = fopen(file, "rb");

    if(!f)
    {
        return false;
    }

    int size;

    if(fread(&size, sizeof(int), 1, f) != 1)
    {
        fclose(f);

        return false;
    }

    for(int i = 0; i < size; ++i)
    {
        BoardLocation location(0, 0);

        if(fread(&location, sizeof(BoardLocation), 1, f) != 1)
        {
            fclose(f);

            return false;
        }

        BlockFinalFeatures features;

        if(fread(&features, sizeof(BlockFinalFeatures), 1, f) != 1)
        {
            fclose(f);

            return false;
        }

        std::pair<BoardLocation, BlockFinalFeatures> mapping(location, features);

        featureMap.insert(mapping);
    }

    fclose(f);

    return true;
}

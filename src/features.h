#ifndef _FEATURES_H_
#define _FEATURES_H_

#define NUMBER_OF_FEATURES 46

typedef struct {
    int NumberOfBlocks;
    int Size;
    int Perimeter;
    int CETNumberOfTerritories;
    int CETSize;
    int CETPerimeter;
    float CETCenterOfMass;
    int ENumberOfBlocks;
    int ESize;
    int EPerimeter;
    int DTNumberOfTerritories;
    int DTDirectLiberties;
    int DTLibertiesOfFriendlyBlocks;
    int DTLibertiesOfEnemyBlocks;
} OptimisticChainFeatures;

typedef struct {
    // Local Features
    int size;
    int perimeter;
    int opponents;
    int liberties;
    int protectedLiberties;
    int autoAtariLiberties;
    int secondOrderLiberties;
    int thirdOrderLiberties;
    int numberOfAdjacentOpponentBlocks;
    int localMajority;
    float centerOfMass;
    int boundingBoxSize;
    // Color Enclosed Territory Features
    int CETNumberOfTerritories;
    int CETSize;
    int CETPerimeter;
    float CETCenterOfMass;
    // Eyespace Features
    int ENumberOfBlocks;
    int ESize;
    int EPerimeter;
    // Optimistic Chain Features
    int OCNumberOfBlocks;
    int OCSize;
    int OCPerimeter;
    int OCCETNumberOfTerritories;
    int OCCETSize;
    int OCCETPerimeter;
    float OCCETCenterOfMass;
    int OCENumberOfBlocks;
    int OCESize;
    int OCEPerimeter;
    int OCDTNumberOfTerritories;
    int OCDTDirectLiberties;
    int OCDTLibertiesOfFriendlyBlocks;
    int OCDTLibertiesOfEnemyBlocks;
    // Weakest Adjacent Enemy Features
    int WAEPerimeter;
    int WAELiberties;
    int WAESharedLiberties;
    // Second Weakest Adjacent Enemy Features
    int SWAEPerimeter;
    int SWAELiberties;
    int SWAESharedLiberties;
    // Weakest Adjacent Chained Enemy Features
    int WACEPerimeter;
    int WACELiberties;
    int WACESharedLiberties;
    // Disputed Territory Features
    int DTNumberOfTerritories;
    int DTDirectLiberties;
    int DTLibertiesOfFriendlyBlocks;
    int DTLibertiesOfEnemyBlocks;
} BlockFinalFeatures;

void printFeatures(const BlockFinalFeatures& features);
float* getFeatureVector(const BlockFinalFeatures& features);

inline
bool operator==(const BlockFinalFeatures& x, const BlockFinalFeatures& y)
{
    float* xv = getFeatureVector(x);
    float* yv = getFeatureVector(y);

    bool result = true;

    for(int i = 0; i < NUMBER_OF_FEATURES; ++i)
    {
        if(xv[i] != yv[i])
        {
            result = false;

            break;
        }
    }

    delete[] xv;
    delete[] yv;

    return result;
}

#endif

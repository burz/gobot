#include "blockFeatures.h"

#include <cstdio>

void printFeatures(const BlockFinalFeatures& features)
{
    printf("Local Features\n--------------\nSize: %d\n", features.size);
    printf("Perimeter: %d\nOpponents: %d\n", features.perimeter, features.opponents);
    printf("Liberties: %d\nProtected Liberties: %d\n",
           features.liberties, features.protectedLiberties);
    printf("Auto-atari Liberties: %d\nSecond-order Liberties: %d\n",
           features.autoAtariLiberties, features.secondOrderLiberties);
    printf("Third-order Liberties: %d\nNumber of Adjacent Opponent Blocks: %d\n",
           features.thirdOrderLiberties, features.numberOfAdjacentOpponentBlocks);
    printf("Local Majority: %d\nCenter of Mass: %f\n",
           features.localMajority, features.centerOfMass);
    printf("Bounding Box Size: %d\n\n", features.boundingBoxSize);
    printf("Color Enclosed Territory Features\n---------------------------------\n");
    printf("Number of Territories: %d\nSize: %d\n",
           features.CETNumberOfTerritories, features.CETSize);
    printf("Perimeter: %d\nCenter of Mass: %f\n\n",
           features.CETPerimeter, features.CETCenterOfMass);
    printf("Eyespace Features\n-----------------\n");
    printf("Number of Blocks: %d\nSize: %d\nPerimeter: %d\n\n",
           features.ENumberOfBlocks, features.ESize, features.EPerimeter);
    printf("Optimistic Chain Features\n-------------------------\n");
    printf("Number of Blocks: %d\nSize: %d\n", features.OCNumberOfBlocks, features.OCSize);
    printf("Perimeter: %d\nNumber of Color Enclosed Territories %d\n",
           features.OCPerimeter, features.OCCETNumberOfTerritories);
    printf("Size of Color Enclosed Territories: %d\n", features.OCCETSize);
    printf("Perimeter of Color Enclosed Territories: %d\n", features.OCCETPerimeter);
    printf("Center of Mass of Color Enclosed Territories: %f\n", features.OCCETCenterOfMass);
    printf("Number of Adjacent Eyespace Blocks: %d\n", features.OCENumberOfBlocks);
    printf("Adjacent Eyspace Size: %d\n", features.OCESize);
    printf("Adjacent Eyespace Perimeter: %d\n", features.OCEPerimeter);
    printf("Number of Disputed Territories: %d\n", features.OCDTNumberOfTerritories);
    printf("Direct Liberties in Disputed Territories: %d\n", features.OCDTDirectLiberties);
    printf("Friendly Liberties in Disputed Territories: %d\n",
           features.OCDTLibertiesOfFriendlyBlocks);
    printf("Enemy Liberties in Disputed Territories: %d\n\n",
           features.OCDTLibertiesOfEnemyBlocks);
    printf("Weakest Adjacent Enemy Features\n-------------------------------\n");
    printf("Perimeter: %d\nLiberties: %d\n", features.WAEPerimeter, features.WAELiberties);
    printf("Shared Liberties: %d\n\n", features.WAESharedLiberties);
    printf("Second Weakest Adjacent Enemy Features\n");
    printf("--------------------------------------\n");
    printf("Perimeter: %d\nLiberties: %d\n", features.SWAEPerimeter, features.SWAELiberties);
    printf("Shared Liberties: %d\n\n", features.SWAESharedLiberties);
    printf("Weakest Adjacent Chained Enemy Features\n");
    printf("---------------------------------------\n");
    printf("Perimeter: %d\nLiberties: %d\n", features.WACEPerimeter, features.WACELiberties);
    printf("Shared Liberties: %d\n\n", features.WACESharedLiberties);
    printf("Disputed Territory Features\n---------------------------\n");
    printf("Number of Territories: %d\nDirect Liberties: %d\n",
           features.DTNumberOfTerritories, features.DTDirectLiberties);
    printf("Liberties of Friendly Blocks: %d\nLiberties of Enemy Blocks: %d\n",
           features.DTLibertiesOfFriendlyBlocks, features.DTLibertiesOfEnemyBlocks);
}

float* getFeatureVector(const BlockFinalFeatures& features)
{
    float* result = new float[NUMBER_OF_FEATURES]();

    // Local Features
    result[0] = features.size;
    result[1] = features.perimeter;
    result[2] = features.opponents;
    result[3] = features.liberties;
    result[4] = features.protectedLiberties;
    result[5] = features.autoAtariLiberties;
    result[6] = features.secondOrderLiberties;
    result[7] = features.thirdOrderLiberties;
    result[8] = features.numberOfAdjacentOpponentBlocks;
    result[9] = features.localMajority;
    result[10] = features.centerOfMass;
    result[11] = features.boundingBoxSize;
    // Eyespace Features
    result[12] = features.ENumberOfBlocks;
    result[13] = features.ESize;
    result[14] = features.EPerimeter;
    // Color Enclosed Territory Features
    result[15] = features.CETNumberOfTerritories;
    result[16] = features.CETSize;
    result[17] = features.CETPerimeter;
    result[18] = features.CETCenterOfMass;
    // Optimistic Chain Features
    result[19] = features.OCNumberOfBlocks;
    result[20] = features.OCSize;
    result[21] = features.OCPerimeter;
    result[22] = features.OCCETNumberOfTerritories;
    result[23] = features.OCCETSize;
    result[24] = features.OCCETPerimeter;
    result[25] = features.OCCETCenterOfMass;
    result[26] = features.OCENumberOfBlocks;
    result[27] = features.OCESize;
    result[28] = features.OCEPerimeter;
    result[29] = features.OCDTNumberOfTerritories;
    result[30] = features.OCDTDirectLiberties;
    result[31] = features.OCDTLibertiesOfFriendlyBlocks;
    result[32] = features.OCDTLibertiesOfEnemyBlocks;
    // Weakest Adjacent Enemy Features
    result[33] = features.WAEPerimeter;
    result[34] = features.WAELiberties;
    result[35] = features.WAESharedLiberties;
    // Second Weakest Adjacent Enemy Features
    result[36] = features.SWAEPerimeter;
    result[37] = features.SWAELiberties;
    result[38] = features.SWAESharedLiberties;
    // Weakest Adjacent Chained Enemy Features
    result[39] = features.WACEPerimeter;
    result[40] = features.WACELiberties;
    result[41] = features.WACESharedLiberties;
    // Disputed Territory Features
    result[42] = features.DTNumberOfTerritories;
    result[43] = features.DTDirectLiberties;
    result[44] = features.DTLibertiesOfFriendlyBlocks;
    result[45] = features.DTLibertiesOfEnemyBlocks;

    return result;
}

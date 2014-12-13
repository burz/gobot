#include "features.h"

#include <stdio.h>

void BlockFinalFeatures::print(void) const
{
    printf("Local Features\n--------------\nSize: %d\n", size);
    printf("Perimeter: %d\nOpponents: %d\n", perimeter, opponents);
    printf("Liberties: %d\nProtected Liberties: %d\n", liberties, protectedLiberties);
    printf("Auto-atari Liberties: %d\nSecond-order Liberties: %d\n",
           autoAtariLiberties, secondOrderLiberties);
    printf("Third-order Liberties: %d\nNumber of Adjacent Opponent Blocks: %d\n",
           thirdOrderLiberties, numberOfAdjacentOpponentBlocks);
    printf("Local Majority: %d\nCenter of Mass: %f\n", localMajority, centerOfMass);
    printf("Bounding Box Size: %d\n\n", boundingBoxSize);
    printf("Color Enclosed Territory Features\n---------------------------------\n");
    printf("Number of Territories: %d\nSize: %d\n", CETNumberOfTerritories, CETSize);
    printf("Perimeter: %d\nCenter of Mass: %f\n\n", CETPerimeter, CETCenterOfMass);
    printf("Optimistic Chain Features\n-------------------------\n");
    printf("Number of Blocks: %d\nSize: %d\n", OCNumberOfBlocks, OCSize);
    printf("Perimeter: %d\nNumber of Color Enclosed Territories %d\n",
           OCPerimeter, OCCETNumberOfTerritories);
    printf("Size of Color Enclosed Territories: %d\n", OCCETSize);
    printf("Perimeter of Color Enclosed Territories: %d\n", OCCETPerimeter);
    printf("Center of Mass of Color Enclosed Territories: %f\n", OCCETCenterOfMass);
    printf("Number of Disputed Territories: %d\n", OCDTNumberOfTerritories);
    printf("Direct Liberties in Disputed Territories: %d\n", OCDTDirectLiberties);
    printf("Friendly Liberties in Disputed Territories: %d\n",
           OCDTLibertiesOfFriendlyBlocks);
    printf("Enemy Liberties in Disputed Territories: %d\n\n",
           OCDTLibertiesOfEnemyBlocks);
    printf("Weakest Adjacent Enemy Features\n-------------------------------\n");
    printf("Perimeter: %d\nLiberties: %d\n", WAEPerimeter, WAELiberties);
    printf("Shared Liberties: %d\n\n", WAESharedLiberties);
    printf("Second Weakest Adjacent Enemy Features\n");
    printf("--------------------------------------\n");
    printf("Perimeter: %d\nLiberties: %d\n", SWAEPerimeter, SWAELiberties);
    printf("Shared Liberties: %d\n\n", SWAESharedLiberties);
    printf("Weakest Adjacent Chained Enemy Features\n");
    printf("---------------------------------------\n");
    printf("Perimeter: %d\nLiberties: %d\n", WACEPerimeter, WACELiberties);
    printf("Shared Liberties: %d\n\n", WACESharedLiberties);
    printf("Disputed Territory Features\n---------------------------\n");
    printf("Number of Territories: %d\nDirect Liberties: %d\n",
           DTNumberOfTerritories, DTDirectLiberties);
    printf("Liberties of Friendly Blocks: %d\nLiberties of Enemy Blocks: %d\n",
           DTLibertiesOfFriendlyBlocks, DTLibertiesOfEnemyBlocks);
}

float* BlockFinalFeatures::getFeatureVector(void) const
{
    float* result = new float[NUMBER_OF_FEATURES]();

    // Local Features
    result[0] = size;
    result[1] = perimeter;
    result[2] = opponents;
    result[3] = liberties;
    result[4] = protectedLiberties;
    result[5] = autoAtariLiberties;
    result[6] = secondOrderLiberties;
    result[7] = thirdOrderLiberties;
    result[8] = numberOfAdjacentOpponentBlocks;
    result[9] = localMajority;
    result[10] = centerOfMass;
    result[11] = boundingBoxSize;
    // Color Enclosed Territory Features
    result[12] = CETNumberOfTerritories;
    result[13] = CETSize;
    result[14] = CETPerimeter;
    result[15] = CETCenterOfMass;
    // Optimistic Chain Features
    result[16] = OCNumberOfBlocks;
    result[17] = OCSize;
    result[18] = OCPerimeter;
    result[19] = OCCETNumberOfTerritories;
    result[20] = OCCETSize;
    result[21] = OCCETPerimeter;
    result[22] = OCCETCenterOfMass;
    result[23] = OCDTNumberOfTerritories;
    result[24] = OCDTDirectLiberties;
    result[25] = OCDTLibertiesOfFriendlyBlocks;
    result[26] = OCDTLibertiesOfEnemyBlocks;
    // Weakest Adjacent Enemy Features
    result[27] = WAEPerimeter;
    result[28] = WAELiberties;
    result[29] = WAESharedLiberties;
    // Second Weakest Adjacent Enemy Features
    result[30] = SWAEPerimeter;
    result[31] = SWAELiberties;
    result[32] = SWAESharedLiberties;
    // Weakest Adjacent Chained Enemy Features
    result[33] = WACEPerimeter;
    result[34] = WACELiberties;
    result[35] = WACESharedLiberties;
    // Disputed Territory Features
    result[36] = DTNumberOfTerritories;
    result[37] = DTDirectLiberties;
    result[38] = DTLibertiesOfFriendlyBlocks;
    result[39] = DTLibertiesOfEnemyBlocks;

    return result;
}

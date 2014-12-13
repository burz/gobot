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
    printf("Optimistic Chain Features\n-------------------------\n");
    printf("Number of Blocks: %d\nSize: %d\n", OCNumberOfBlocks, OCSize);
    printf("Perimeter: %d\n\n", OCPerimeter);
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
    printf("Shared Liberties: %d\n", WACESharedLiberties);
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
    // Optimistic Chain Features
    result[12] = OCNumberOfBlocks;
    result[13] = OCSize;
    result[14] = OCPerimeter;
    // Weakest Adjacent Enemy Features
    result[15] = WAEPerimeter;
    result[16] = WAELiberties;
    result[17] = WAESharedLiberties;
    // Second Weakest Adjacent Enemy Features
    result[18] = SWAEPerimeter;
    result[19] = SWAELiberties;
    result[20] = SWAESharedLiberties;
    // Weakest Adjacent Chained Enemy Features
    result[21] = WACEPerimeter;
    result[22] = WACELiberties;
    result[23] = WACESharedLiberties;
    // Disputed Territory Features
    result[24] = DTNumberOfTerritories;
    result[25] = DTDirectLiberties;
    result[26] = DTLibertiesOfFriendlyBlocks;
    result[27] = DTLibertiesOfEnemyBlocks;

    return result;
}

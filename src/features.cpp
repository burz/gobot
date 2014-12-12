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
    printf("Bounding Box Size: %d\n\nOptimistic Chain Features\n", boundingBoxSize);
    printf("-------------------------\nNumber of Blocks: %d\n", OCNumberOfBlocks);
    printf("Size: %d\nPerimeter: %d\n", OCSize, OCPerimeter);
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

    return result;
}

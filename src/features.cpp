#include "features.h"

#include <stdio.h>

void BlockFinalFeatures::print(void) const
{
    printf("Size: %d\nPerimeter: %d\nOpponents: %d\n", size, perimeter, opponents);
    printf("Liberties: %d\nProtected Liberties: %d\n", liberties, protectedLiberties);
    printf("Auto-atari Liberties: %d\nSecond-order Liberties: %d\n",
           autoAtariLiberties, secondOrderLiberties);
    printf("Third-order Liberties: %d\nNumber of Adjacent Opponent Blocks: %d\n",
            thirdOrderLiberties, numberOfAdjacentOpponentBlocks);
    printf("Local Majority: %d\nCenter of Mass: %f\n", localMajority, centerOfMass);
    printf("Bounding Box Size: %d\n", boundingBoxSize);
}

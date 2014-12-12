#ifndef _FEATURES_H_
#define _FEATURES_H_

#define NUMBER_OF_FEATURES 15

class BlockFinalFeatures
{
  public:
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
    // Optimistic Chain Features
    int OCNumberOfBlocks;
    int OCSize;
    int OCPerimeter;

    void print(void) const;
    float* getFeatureVector(void) const;
};

#endif

#ifndef _FEATURES_H_
#define _FEATURES_H_

class BlockFinalFeatures
{
  public:
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

    void print(void) const;
};

#endif

#ifndef _RPROP_WEIGHTS_H_
#define _RPROP_WEIGHTS_H_

#include <cstdio>

class Weights
{
  private:
    int height;
    int width;
    float** weights;

    float** derivative;
    float** delta;
    float** deltaW;

    float deltaMin;
    float deltaMax;

    float** lastDerivative;
    float** lastDelta;
    float** lastDeltaW;

    virtual void calculateDerivatives(float* features) = 0;

    float updateElement(const int& i, const int& j);
  public:
    Weights(const int& height, const int& width);
    ~Weights(void);

    int getHeight(void) const;
    float getWeight(const int& i, const int& j) const;

    void initializeForTraining(void);
    void cleanUpAfterTraining(void);

    void update(float* features);

    bool writeToFile(FILE* file) const;
    bool readFromFile(FILE* file);
};

#endif

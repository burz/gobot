#ifndef _RPROP_WEIGHTS_H_
#define _RPROP_WEIGHTS_H_

#include <cstdio>

class RProp;

class Weights
{
  protected:
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

    virtual void calculateDerivatives(RProp& model,
                                      float* features,
                                      const bool& alive) = 0;

    float updateElement(RProp& model, const int& i, const int& j);
  public:
    Weights(const int& height, const int& width);
    ~Weights(void);

    int getHeight(void) const;
    int getWidth(void) const;

    float getWeight(const int& i, const int& j = 0) const;

    void initializeForTraining(void);
    void cleanUpAfterTraining(void);

    void update(RProp& model, float* features, const bool& alive);

    bool writeToFile(FILE* file) const;
    bool readFromFile(FILE* file);
};

#endif

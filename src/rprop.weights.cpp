#include "rprop.weights.h"

#include <cmath>
#include <cstdlib>

#define DELTA0 0.1
#define ETAPLUS 1.2
#define ETAMINUS 0.5

Weights::Weights(const int& _height, const int& _width)
{
    height = _height;
    width = _width;

    if(height)
    {
        float** weights = new float*[height];

        for(int i = 0; i < height; ++i)
        {
            weights[i] = new float[width];
        }
    }
}

Weights::~Weights(void)
{
    if(height)
    {
        for(int i = 0; i < height; ++i)
        {
            delete[] weights[i];
        }

        delete weights;
    }
}

int Weights::getWidth(void) const
{
    return width;
}

int Weights::getHeight(void) const
{
    return height;
}

float Weights::getWeight(const int& i, const int& j) const
{
    return weights[i][j];
}

inline
float randomFloatAroundZero(void)
{
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 2.0) - 1.0;
}

void Weights::initializeForTraining(void)
{
    derivative = new float*[height];
    delta = new float*[height];
    deltaW = new float*[height];
    lastDerivative = new float*[height];
    lastDelta = new float*[height];
    lastDeltaW = new float*[height];

    for(int i = 0; i < height; ++i)
    {
        derivative[i] = new float[width];
        delta[i] = new float[width];
        deltaW[i] = new float[width];
        lastDerivative[i] = new float[width];
        lastDelta[i] = new float[width];
        lastDeltaW[i] = new float[width];

        for(int j = 0; j < width; ++j)
        {
            weights[i][j] = randomFloatAroundZero();

            derivative[i][j] = 1.0;
            delta[i][j] = DELTA0;
            deltaW[i][j] = DELTA0;
        }
    }
}

void Weights::cleanUpAfterTraining(void)
{
    for(int i = 0; i < height; ++i)
    {
        delete[] derivative[i];
        delete[] delta[i];
        delete[] deltaW[i];
        delete[] lastDerivative[i];
        delete[] lastDelta[i];
        delete[] lastDeltaW[i];
    }

    delete[] derivative;
    delete[] delta;
    delete[] deltaW;
    delete[] lastDerivative;
    delete[] lastDelta;
    delete[] lastDeltaW;
}

inline
float sign(const float& x)
{
    if(x == 0.0)
    {   
        return 0.0;
    }   
    else if(x > 0.0)
    {   
        return 1.0;
    }   
    else
    {   
        return -1.0;
    }   
}

float Weights::updateElement(const int& i, const int& j)
{
    float d = derivative[i][j] * lastDerivative[i][j]; 

    if(d > 0.0)
    {   
        delta[i][j] = fmin(ETAPLUS * lastDelta[i][j], deltaMax);

        deltaW[i][j] = -sign(derivative[i][j]) * delta[i][j];

        return weights[i][j] + deltaW[i][j];
    }   
    else if(d < 0.0)
    {   
        delta[i][j] = fmax(ETAMINUS * lastDelta[i][j], deltaMin);

        derivative[i][j] = 0.0;

        return weights[i][j] - lastDeltaW[i][j];
    }
    else
    {
        delta[i][j] = lastDelta[i][j];

        deltaW[i][j] = -sign(derivative[i][j]) * delta[i][j];

        return weights[i][j] + deltaW[i][j];
    }
}

inline
void swapPointers(float**& x, float**& y)
{
    float** t = y;

    y = x;
    x = t;
}

void Weights::update(float* features)
{
    swapPointers(derivative, lastDerivative);
    swapPointers(delta, lastDelta);
    swapPointers(deltaW, lastDeltaW);

    calculateDerivatives(features);

    for(int i = 0; i < height; ++i)
    {
        for(int j = 0; j < width; ++j)
        {
            weights[i][j] = updateElement(i, j);
        }
    }

    deltaMin = deltaMax = delta[0][0];

    for(int i = 0; i < height; ++i)
    {
        for(int j = 0; j < width; ++j)
        {
            if(delta[i][j] < deltaMin)
            {
                deltaMin = delta[i][j];
            }
            else if(delta[i][j] > deltaMax)
            {
                deltaMax = delta[i][j];
            }
        }
    }
}

bool Weights::writeToFile(FILE* file) const
{
    if(fwrite(&height, sizeof(int), 1, file) != 1 ||
       fwrite(&width, sizeof(int), 1, file) != 1)
    {
        return false;
    }

    for(int i = 0; i < height; ++i)
    {
        for(int j = 0; j < width; ++j)
        {
            if(fwrite(&weights[i][j], sizeof(float), 1, file) != 1)
            {
                return false;
            }
        }
    }

    return false;
}

bool Weights::readFromFile(FILE* file)
{
    if(fread(&height, sizeof(int), 1, file) != 1 ||
       fread(&width, sizeof(int), 1, file) != 1)
    {
        return false;
    }

    for(int i = 0; i < height; ++i)
    {
        for(int j = 0; j < width; ++j)
        {
            if(fread(&weights[i][j], sizeof(float), 1, file) != 1)
            {
                return false;
            }
        }
    }

    return false;
}

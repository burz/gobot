#include "rprop.layers.h"
#include "rprop.h"

void InputLayer::calculateDerivatives(RProp& model, float* features, const bool& alive)
{
    float t = alive == true ? 1.0 : -1.0;
    float r = model.calculateR(features);
    float R = t - r;

    for(int i = 0; i < height; ++i)
    {
        float h = model.hiddenLayer.getWeight(i);

        for(int j = 0; j < width; ++j)
        {
            float fa = features[j] + model.inputBias.getWeight(j);

            derivative[i][j] = -1.0 * R * h * fa;
        }
    }
}

void InputBias::calculateDerivatives(RProp& model, float* features, const bool& alive)
{
    float t = alive == true ? 1.0 : -1.0;
    float r = model.calculateR(features);
    float R = t - r;

    for(int i = 0; i < height; ++i)
    {
        float sum = 0.0;

        for(int j = 0; j < model.hiddenSize; ++j)
        {
            sum += model.hiddenLayer.getWeight(j) * model.inputLayer.getWeight(i, j);
        }

        derivative[i][0] = -1.0 * R * sum;
    }
}

void HiddenLayer::calculateDerivatives(RProp& model, float* features, const bool& alive)
{
    float t = alive == true ? 1.0 : -1.0;
    float r = model.calculateR(features);
    float R = t - r;

    for(int i = 0; i < height; ++i)
    {
        float sum = model.hiddenBias.getWeight(i);

        for(int j = 0; j < model.inputSize; ++j)
        {
            float fa = features[j] + model.inputBias.getWeight(j);

            sum += model.inputLayer.getWeight(i, j) * fa;
        }

        derivative[i][0] = R * sum;
    }
}

void HiddenBias::calculateDerivatives(RProp& model, float* features, const bool& alive)
{
    float t = alive == true ? 1.0 : -1.0;
    float r = model.calculateR(features);
    float R = t - r;

    for(int i = 0; i < height; ++i)
    {
        derivative[i][0] = R * model.hiddenLayer.getWeight(i);
    }
}

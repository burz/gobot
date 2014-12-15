#ifndef _RPROP_H_
#define _RPROP_H_

#include "model.h"
#include "directoryLoader.h"

#include <vector>
#include <map>

#define SECOND_INPUT_SIZE 5
#define SECOND_HIDDEN_SIZE 4

typedef enum {
    INPUT,
    HIDDEN,
    INPUT_BIAS,
    HIDDEN_BIAS,
    LAST_INPUT,
    LAST_HIDDEN,
    LAST_INPUT_BIAS,
    LAST_HIDDEN_BIAS,
    SECOND_INPUT,
    SECOND_HIDDEN,
    SECOND_INPUT_BIAS,
    SECOND_HIDDEN_BIAS,
    LAST_SECOND_INPUT,
    LAST_SECOND_HIDDEN,
    LAST_SECOND_INPUT_BIAS,
    LAST_SECOND_HIDDEN_BIAS
} ParameterType;

class RProp : public Model
{
  private:
    int inputSize;
    int hiddenSize;

    float** inputLayer;
    float* hiddenLayer;
    float* inputBias;
    float* hiddenBias;

    float** secondInputLayer;
    float* secondHiddenLayer;
    float* secondInputBias;
    float* secondHiddenBias;

    // Learning parameters
    float** inputDerivative;
    float** inputDelta;
    float** inputDeltaW;
    float** lastInputDerivative;
    float** lastInputDelta;
    float** lastInputDeltaW;

    float* hiddenDerivative;
    float* hiddenDelta;
    float* hiddenDeltaW;
    float* lastHiddenDerivative;
    float* lastHiddenDelta;
    float* lastHiddenDeltaW;

    float* inputBiasDerivative;
    float* inputBiasDelta;
    float* inputBiasDeltaW;
    float* lastInputBiasDerivative;
    float* lastInputBiasDelta;
    float* lastInputBiasDeltaW;

    float* hiddenBiasDerivative;
    float* hiddenBiasDelta;
    float* hiddenBiasDeltaW;
    float* lastHiddenBiasDerivative;
    float* lastHiddenBiasDelta;
    float* lastHiddenBiasDeltaW;

    float** secondInputDerivative;
    float** secondInputDelta;
    float** secondInputDeltaW;
    float** lastSecondInputDerivative;
    float** lastSecondInputDelta;
    float** lastSecondInputDeltaW;

    float* secondHiddenDerivative;
    float* secondHiddenDelta;
    float* secondHiddenDeltaW;
    float* lastSecondHiddenDerivative;
    float* lastSecondHiddenDelta;
    float* lastSecondHiddenDeltaW;

    float* secondInputBiasDerivative;
    float* secondInputBiasDelta;
    float* secondInputBiasDeltaW;
    float* lastSecondInputBiasDerivative;
    float* lastSecondInputBiasDelta;
    float* lastSecondInputBiasDeltaW;

    float* secondHiddenBiasDerivative;
    float* secondHiddenBiasDelta;
    float* secondHiddenBiasDeltaW;
    float* lastSecondHiddenBiasDerivative;
    float* lastSecondHiddenBiasDelta;
    float* lastSecondHiddenBiasDeltaW;

    float deltaMin;
    float deltaMax;

    void initializeTrainingParameters(void);
    void cleanUpTrainingParameters(void);

    float derivative(const ParameterType& type, const int& i, const int& j);
    float delta(const ParameterType& type, const int& i, const int& j);
    float deltaW(const ParameterType& type, const int& i, const int& j);
    float w(const ParameterType& type, const int& i, const int& j);

    void setDerivative(const ParameterType& type,
                       const int& i, const int& j, const float& d);
    void setDelta(const ParameterType& type,
                  const int& i, const int& j, const float& d);
    void setDeltaW(const ParameterType& type,
                    const int& i, const int& j, const float& d);

    float updateWeight(const ParameterType& type, const int& i, const int& j = 0);

    float calculateR(float* features) const;
    float calculateS(float* secondFeatures) const;

    void calculateDerivatives(const Game& game,
                              const Board& board,
                              std::vector<Block*>& emptyBlocks,
                              std::vector<Block*>& coloredBlocks,
                              std::map<Block*, float*>& featureMap);

    void runUpdates(const Game& game,
                    const Board& board,
                    std::vector<Block*>& emptyBlocks,
                    std::vector<Block*>& coloredBlocks,
                    std::map<Block*, float*>& featureMap);

    float runSecondPart(const Game& game,
                        const Board& board,
                        std::set<Block*>& blocks,
                        std::map<Block*, float>& resultMap,
                        std::set<Block*>& emptyBlocks) const;
  public:
    RProp(void);
    RProp(const int& inputSize, const int& hiddenSize);
    ~RProp(void);

    float energyFunction(const Game& game) const;

    void train(DirectoryIterator& gameFiles);
    virtual void train(std::vector<Game>& games, const int& iterations);
    void trainWithFeatures(DirectoryIterator& gameFiles,
                           const char* featureFileDirectory);

    virtual float predict(const Game& game) const;
    float predictWithFeatures(
            const Game& game,
            std::map<BoardLocation, BlockFinalFeatures>& featureMap) const;

    float test(DirectoryIterator& gameFiles) const;
    virtual float test(std::vector<Game>& games) const;
    float testWithFeatures(DirectoryIterator& gameFiles,
                           const char* featureFileDirectory) const;

    virtual bool outputToFile(const char* filename) const;
    virtual bool readFromFile(const char* filename);
};

#endif

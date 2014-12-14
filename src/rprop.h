#ifndef _RPROP_H_
#define _RPROP_H_

#include "model.h"

class RProp : public Model
{
  private:
    int inputSize;
    int hiddenSize;
    float** bottomLayer;
    float* hiddenLayer;

  public:
    RProp(void);
    RProp(const int& inputSize, const int& hiddenSize);
    ~RProp(void);

    virtual void train(std::vector<Game>& games);
    virtual float test(std::vector<Game>& games) const;
    virtual float run(const Game& game) const;

    virtual bool outputToFile(const char* filename) const;
    virtual bool readFromFile(const char* filename);
};

#endif

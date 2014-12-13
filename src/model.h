#ifndef _MODEL_H_
#define _MODEL_H_

class Model
{
  private:
  public:
    void train(std::vector<Game>& games) = 0;
    float run(Game game) const = 0;
    float test(std::vector<Game>& games) = 0;
    bool outputToFile(const char* filename) const = 0;
};

#endif

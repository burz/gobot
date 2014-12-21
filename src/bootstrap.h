#ifndef _BOOTSTRAP_H_
#define _BOOTSTRAP_H_

#include "Board.h"
#include "rprop.h"

class Bootstrap
{
  private:
    const char* sourceDirectory;
    const char* destinationDirectory;
    const char* labelDirectory;

    bool manuallyLabelBoard(const char* boardFile) const;
    bool automaticallyLabelBoard(const RProp& model, const char* boardFile) const;
  public:
    Bootstrap(const char* sourceDirectory,
              const char* destinationDirectory,
              const char* labelDirectory);

    void printBoard(const Board& board,
                    std::map<Block*, bool>& lifeMap,
                    std::map<BoardLocation, bool>& territoryMap,
                    Block* selectedBlock = 0) const;

    void run(RProp& model) const;
};

#endif

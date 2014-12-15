#ifndef _BOARD_H_
#define _BOARD_H_

#include "space.h"
#include "block.h"

#include <vector>

typedef struct {
    std::set<BoardLocation> perimeter;
    std::set<BoardLocation> liberties;
    std::set<BoardLocation> opponents;
    std::set<BoardLocation> secondOrderLiberties;
    std::set<BoardLocation> thirdOrderLiberties;
    std::set<Block*> adjacentOpponentBlocks;
    Block* weakestAdjacentChainedBlock;
    std::set<BoardLocation> friendly;
    std::set<BoardLocation> enemy;
    std::set<Block*> adjacentTerritories;
    std::set<Block*> optimisticChain;
    std::set<Block*> adjacentChainedTerritories;
} LocalFeatureState;

class Board
{
  private:
    const int size;
    float score;
    Space** spaces;
    bool modified;
    std::vector<std::set<Block*> > optimisticChains;
    std::vector<OptimisticChainFeatures> optimisticChainFeatures;

    void recalculateLiberties(Block* block);
    bool handleAdjacentBlock(Block* currentBlock, Block* block);
    void removeDeadGroup(Block* block);
    void handlePossiblyDeadBlocks(Block* block1,
                                  Block* block2,
                                  Block* block3,
                                  Block* block4);

    bool isFalseEye(Block* block) const;
    void handleAdjacentTerritories(std::set<Block*>& chain,
                                   std::set<Block*>& adjacentTerritories,
                                   int& CETNumberOfTerritories,
                                   int& CETSize,
                                   int& CETPerimeter,
                                   float& CETCenterOfMass,
                                   int& ENumberOfBlocks,
                                   int& ESize,
                                   int& EPerimeter,
                                   int& DTNumberOfTerritories,
                                   int& DTDirectLiberties,
                                   int& DTLibertiesOfFriendlyBlocks,
                                   int& DTLibertiesOfEnemyBlocks) const;
    void calculateSecondOrderLiberties(LocalFeatureState* state,
                                       Block* block0,
                                       Block* block,
                                       const int x,
                                       const int y,
                                       bool& autoAtari,
                                       std::vector<Block*>& optimisticList) const;
    int lookupBlockInChainMap(Block* block) const;
    void calculateOptimisticChain(LocalFeatureState* state,
                                  Block* block0,
                                  Block* block,
                                  const int x,
                                  const int y,
                                  std::set<BoardLocation>& perimeter,
                                  std::vector<Block*>& optimisticList) const;
    void generateOptimisticChain(BlockFinalFeatures *features,
                                 LocalFeatureState* state,
                                 Block* block,
                                 std::vector<Block*>& optimisticList) const;
    bool isProtected(Block* block, const int x, const int y) const;
    void calculateWeakEnemyFeatures(int& perimeter,
                                    int& liberties,
                                    int& sharedLiberties,
                                    Block* block0,
                                    Block* block) const;
    void generateWeakestEnemyFeatures(BlockFinalFeatures *features,
                                      LocalFeatureState* state,
                                      Block* block) const;
    void generateLocalFeatures(BlockFinalFeatures *features, Block* block);
  public:
    Board(const int size, const float komi);
    ~Board(void);

    int getSize(void) const;

    float getScore(void) const;

    SpaceState getState(const int x, const int y) const;

    void playMove(const int x, const int y, const SpaceState state);

    Block* getBlock(const int x, const int y) const;
    void setBlock(const int x, const int y, Block* block);
    void setBlock(const BoardLocation& location, Block* block) const;
    void changeBlocks(Block* from, Block* to);

    void splitEmptyBlocks(void);

    void getBlocks(std::set<Block*>& blocks) const;
    void getAdjacentBlocks(std::set<Block*>& blocks, Block* block) const;

    void print(void) const;

    BlockFinalFeatures generateFinalFeatures(Block* block);
    float* generateFinalFeatureVector(Block* block);
};

#endif

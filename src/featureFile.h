#ifndef _FEATUREFILE_H_
#define _FEATUREFILE_H_

#include "board.h"

#include <map>

bool writeFeaturesToFile(Board& board, const char* destination);
bool readFeaturesFromFile(std::map<BoardLocation, BlockFinalFeatures>& featureMap,
                          const char* file);

#endif

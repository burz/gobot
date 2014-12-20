#ifndef _LIFE_FILE_H_
#define _LIFE_FILE_H_

#include "block.h"

#include <map>

bool writeLifeFile(std::map<Block*, bool>& lifeMap);
bool readLifeFile(std::map<BoardLocation, bool>& lifeMap);

#endif

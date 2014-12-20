#ifndef _LIFE_FILE_H_
#define _LIFE_FILE_H_

#include "block.h"

#include <map>

bool writeLifeFile(std::map<Block*, bool>& lifeMap, const char* filename);
bool readLifeFile(std::map<BoardLocation, bool>& lifeMap, const char* filename);

#endif

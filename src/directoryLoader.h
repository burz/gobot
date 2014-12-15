#ifndef _DIRECTORYLOADER_H_
#define _DIRECTORYLOADER_H_

#include "game.h"

#include <dirent.h>

bool loadDirectory(std::vector<Game>& games, const char* directory);

class DirectoryIterator
{
  private:
    const char* directory;
    int maxLoops;
    int currentLoop;

    DIR* dir;
    struct dirent *ent;

    DirectoryIterator(void);
  public:
    DirectoryIterator(const char* directory, const int& maxLoops = 1);
    ~DirectoryIterator(void);

    const char* getDirectory(void) const;

    const char* operator*(void);
    DirectoryIterator& operator++(void);

    static DirectoryIterator end(void);

    friend bool operator!=(const DirectoryIterator& x, const DirectoryIterator& y);
};

inline
bool operator!=(const DirectoryIterator& x, const DirectoryIterator& y)
{
    return x.dir != y.dir;
}

#endif

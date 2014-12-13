#ifndef _DIRECTORYLOADER_H_
#define _DIRECTORYLOADER_H_

#include "game.h"

#include <dirent.h>

bool loadDirectory(std::vector<Game>& games, const char* directory);

class DirectoryIterator
{
  private:
    DIR* dir;
    struct dirent *ent;
    char directory[100];

    DirectoryIterator(void);
  public:
    DirectoryIterator(const char* directory);
    ~DirectoryIterator(void);

    Game operator*(void);
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

#include "space.h"

namespace {
const char empty[] = "EMPTY";
const char black[] = "BLACK";
const char white[] = "WHITE";
} // close anonymous namespace

const char* const spaceStateString(const SpaceState state)
{
    if(state == EMPTY) {
        return empty;
    }   
    else if(state == BLACK) {
        return black;
    }   
    else {
        return white;
    }   
}

Space::Space(void)
{
    state = EMPTY;
}

SpaceState Space::getState(void) const
{
    return state;
}

void Space::changeState(const SpaceState _state)
{
    state = _state;
}

#ifndef UTIL_H
#define UTIL_H

#include <SDL3/SDL.h>

struct GameUtil {
    static inline float deltaTime = 0.1f;
    void update();
private:
    Uint64 lastTime = SDL_GetTicksNS();
};

#endif UTIL_H
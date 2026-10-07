#include "header/util.hpp"

void GameUtil::update() {
    Uint64 currentTime = SDL_GetTicksNS();
    deltaTime = (float)(currentTime - lastTime) / SDL_NS_PER_SECOND;
    lastTime = currentTime;
}
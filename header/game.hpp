#ifndef GAME_H
#define GAME_H

#include <glm/glm.hpp>
#include "util.hpp"
#include "render.hpp"
#include "town.hpp"
#include "runWorld.hpp"

struct GlobalGameData {
};

class Game {
private:
    SDL_Event event;
    Camera camera;
    TownGameData townGameData;
    RunGameData runGameData;
    Renderer renderer;
    GameUtil gameUtil;
    bool inTown = true;
    void beginRunGame();
    void beginTown();
    void updateTown();
    void updateRun();
public:
    Game();
    bool update();
    void quit();
};

#endif GAME_H
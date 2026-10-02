#ifndef GAME_H
#define GAME_H

#include <glm/glm.hpp>
#include "render.hpp"
#include "entity.hpp"

struct GameUtil {
    static inline float deltaTime = 0.1f;
    void update();
private:
    Uint64 lastTime = SDL_GetTicksNS();
};

struct GameObjects {
    Player player;
    std::vector<Tree> trees;
    GameObjects();
};

class Game {
private:
    SDL_Event event;
    Camera camera;
    GameObjects gameObjects;
    Renderer renderer;
    GameUtil gameUtil;
public:
    Game();
    bool update();
    void quit();
};

#endif GAME_H
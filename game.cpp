#include "header/game.hpp"
#include "header/render.hpp"

#include <glm/gtc/matrix_transform.hpp>

void GameUtil::update() {
    Uint64 currentTime = SDL_GetTicksNS();
    deltaTime = (float)(currentTime - lastTime) / SDL_NS_PER_SECOND;
    lastTime = currentTime;
}

GameObjects::GameObjects() {
}

Game::Game() : renderer(camera, gameObjects) {

}

bool Game::update() {
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            return false;
        }
    }
    gameUtil.update();

    gameObjects.player.update();

    renderer.updateRendering();

    return true;
}

void Game::quit() {
    renderer.cleanup();
}
#include "header/game.hpp"
#include "header/render.hpp"

#include <glm/gtc/matrix_transform.hpp>

Game::Game() : renderer(camera, townGameData, runGameData) {

}

bool Game::update() {
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            return false;
        }
        if(event.type == SDL_EVENT_WINDOW_RESIZED) {
            renderer.updateWindowSize();
        }
    }
    gameUtil.update();

    if(inTown) {
        updateTown();
    } else {
        updateRun();
    }

    renderer.updateRendering();

    return true;
}

void Game::updateTown() {
    townGameData.player.update();

    camera.position = glm::vec3(townGameData.player.position.x, 4, townGameData.player.position.z + 3);

    if (townGameData.exit.shouldExit(townGameData.player.position)) {
        beginRunGame();
    }
}

void Game::updateRun() {
    runGameData.player.update();
    camera.position = glm::vec3(runGameData.player.position.x, 4, runGameData.player.position.z + 3);
}

void Game::quit() {
    renderer.cleanup();
}

void Game::beginRunGame() {
    inTown = false;
    camera.initializeRunView();
    renderer.isPerspective = true;
    renderer.renderingTown = false;
    renderer.updateWindowSize();
}

void Game::beginTown() {
    inTown = true;
    camera.initializeTownView();
    renderer.isPerspective = false;
    renderer.renderingTown = true;
    renderer.updateWindowSize();
}
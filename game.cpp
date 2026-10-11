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

    for(Building &building : townGameData.buildings) {
        building.collider.preventCollision(townGameData.player.collider, townGameData.player.position, townGameData.player.velocity);
    }

    camera.position.x = townGameData.player.position.x;
    camera.position.z = townGameData.player.position.z + 3;

    if (townGameData.exit.shouldExit(townGameData.player.position)) {
        beginRunGame();
    }
}

void Game::updateRun() {
    camera.position = glm::vec3(runGameData.player.position.x, runGameData.player.position.y, runGameData.player.position.z);
    runGameData.update();
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
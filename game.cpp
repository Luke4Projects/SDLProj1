#include "header/game.hpp"
#include "header/render.hpp"

#include <glm/gtc/matrix_transform.hpp>

void GameUtil::update() {
    Uint64 currentTime = SDL_GetTicksNS();
    deltaTime = (float)(currentTime - lastTime) / SDL_NS_PER_SECOND;
    lastTime = currentTime;
}

GameObjects::GameObjects() {
    for(int i = 0; i < 20; i++) {
        //Tree tree(glm::vec3(SDL_rand(10) - 5, 1, SDL_rand(5)-5));
        //trees.push_back(tree);
    }
    for(int i = 0; i < 20; i++) {
    }

    int iterations = 0;
    glm::ivec2 position = glm::ivec2(0,0);
    glm::ivec2 atlasPos = glm::ivec2(2,1);
    float rotation = glm::radians(90.0f);
    while(iterations < 10) {
        int amountUp = 3;
        for(int i = 0; i < amountUp; i++) {
            // create road going up (z)
            position.y--;
            Entity tile(glm::vec3(position.x, 0.01f, position.y), glm::vec3(1), glm::vec3(rotation, 0, 0), atlasPos);
            tileSegments.push_back(tile);
        }
        // create house
        Building building(glm::vec3(position.x, 1.44f, position.y - 0.3f), SDL_rand(4));
        buildings.push_back(building);
        // change direction
        int xDir = SDL_rand(2)*2 - 1;
        SDL_Log("%d",xDir);
        int amountSide = 3;
        for(int i = 0; i < amountSide; i++) {
            // create road going side ways
            position.x += xDir;
            Entity tile(glm::vec3(position.x, 0.01f, position.y), glm::vec3(1), glm::vec3(rotation, 0, 0), atlasPos);
            tileSegments.push_back(tile);
        }
        iterations++;
    }

    //for(int i = 0; i < 5; i++) {
    //    glm::ivec2 atlasPos = glm::ivec2(2,3);
    //    if(i == 0) {
    //        atlasPos.x = 1;
    //    }
    //    if(i == 4) {
    //        atlasPos.x = 3;
    //    }
    //    Entity segment(glm::vec3(i, 0.01f, 0), glm::vec3(1), glm::vec3(glm::radians(90.0f), 0, 0), atlasPos);
    //    tileSegments.push_back(segment);
    //}

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
        if(event.type == SDL_EVENT_WINDOW_RESIZED) {
            renderer.updateWindowSize();
        }
    }
    gameUtil.update();

    gameObjects.player.update();

    camera.setPosition(glm::vec3(gameObjects.player.position.x, 4, gameObjects.player.position.z+3));

    renderer.updateRendering();

    return true;
}

void Game::quit() {
    renderer.cleanup();
}